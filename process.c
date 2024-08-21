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

	index = 0;
	while (index < shell->p_size && shell->p[index].pid)
	{
		waitpid(shell->p[index].pid, &status, 0);
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
	if (!ft_memcmp(shell->p[0].args[0], "exit", 5) && !status)
	{
		clean_files(shell->p, shell->p_size);
		exit_wo_error(shell, g_status);
	}
	g_status = status;
}

static int	token_to_process(t_shell *shell, t_token *t, size_t *index)
{
	int	status;

	status = EXIT_SUCCESS;
	if (t->type == T_SIMPLE_CMD)
	{
		if (set_args(&shell->p[*index], shell->data, t->right))
			return (EXTRA_ERROR);
		status = find_redirect(&shell->p[*index], t->left);
		shell->p[*index].index = *index;
		*index += 1;
		return (status);
	}
	if (t->left)
		status = token_to_process(shell, t->left, index);
	if (!status && t->right)
		status = token_to_process(shell, t->right, index);
	return (status);
}

void	exec_cmds(t_shell *shell)
{
	size_t	index;
	int		status;

	index = 0;
	shell->p_size = find_pipe(shell->t);
	shell->p = ft_calloc(shell->p_size + 1, sizeof(t_process));
	status = set_env_paths(&shell->data);
	if (shell->p)
		status = token_to_process(shell, shell->t, &index);
	if (!shell->p || status)
	{
		if (status == EXTRA_ERROR)
			handle_error(NULL, NULL, EXTRA_ERROR);
		g_status = EXIT_FAILURE;
	}
	else if (shell->p_size == 1 && is_builtin(shell->p[0].path))
		inprocess(shell);
	else
		subprocess(shell);
}
