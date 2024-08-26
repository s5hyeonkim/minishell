/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 07:02:29 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/19 18:27:46 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static void	wait_process(t_shell *shell)
{
	size_t	index;
	int		status;
	int		ret;

	index = 0;
	while (index < shell->p_size && shell->p[index].pid)
	{
		ret = waitpid(shell->p[index].pid, &status, 0);
		if (ret == -1)
			continue ;
		if (!WIFSIGNALED(status) && WIFEXITED(status))
			g_status = WEXITSTATUS(status);
		index++;
	}
}

static void	subprocess(t_shell *shell)
{
	size_t	index;

	index = 0;
	set_signal_sub(handler_sub);
	while (index < shell->p_size)
	{
		if (open_pipe(&shell->p[index], shell->p_size) \
				|| fork_process(&shell->p[index]))
		{
			handle_error(NULL, NULL, EXTRA_ERROR);
			break ;
		}
		child(shell, index);
		parent(shell, index);
		index++;
	}
	wait_process(shell);
	set_signal_init(handler_init);
}

static void	inprocess(t_shell *shell)
{
	long	status;

	set_rwfd(shell->p);
	status = exec_builtin(shell->p[0], &shell->data);
	clean_files(shell->p, shell->p_size);
	if (!ft_memcmp(shell->p[0].exec.args[0], "exit", 5))
	{
		ft_putstr_fd("exit\n", STDERR_FILENO);
		if (!status)
			exit_wo_error(shell, g_status);
	}
	g_status = status;
}

static void	token_to_process(t_shell *shell, t_token *t, size_t *index)
{
	if (t->type == T_SIMPLE_CMD)
	{
		shell->p[*index].index = *index;
		shell->p[*index].t = *t;
		*index += 1;
		return ;
	}
	if (t->left)
		token_to_process(shell, t->left, index);
	if (t->right)
		token_to_process(shell, t->right, index);
}

int	set_cmd(t_shell *shell)
{
	size_t	index;

	index = 0;
	if (set_env_paths(&shell->data))
		return (EXTRA_ERROR);
	while (index < shell->p_size)
	{
		if (set_args(&shell->p[index], shell->data))
			return (EXTRA_ERROR);
		if (set_redirect(&shell->p[index], shell->data.envps))
			return (EXTRA_ERROR);
		index++;
	}
	return (EXIT_SUCCESS);
}

void	exec_cmds(t_shell *shell)
{
	size_t	index;

	index = 0;
	shell->p_size = find_pipe(shell->t);
	shell->p = ft_calloc(shell->p_size + 1, sizeof(t_process));
	if (shell->p)
		token_to_process(shell, shell->t, &index);
	if (!shell->p || set_cmd(shell))
		g_status = handle_error(NULL, NULL, EXTRA_ERROR);
	else if (shell->p_size == 1 && is_builtin(shell->p[0].exec.path))
		inprocess(shell);
	else
		subprocess(shell);
}
