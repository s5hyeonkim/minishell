/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 07:02:29 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/14 07:05:04 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	wait_process(t_shell *shell)
{
	int	index;
	int	status;

	index = 0;
	while (shell->p[index].path && shell->p[index].pid)
	{
		waitpid(shell->p[index].pid, &status, 0);
		if (WIFEXITED(status))
			g_status = WEXITSTATUS(status);
		index++;
	}
}

void	subprocess(t_shell *shell)
{
	size_t	index;

	index = 0;
	set_signal_sub(shell, handler_sub);
	while (index < shell->p_size)
	{
		if (open_pipe(&shell->p[index], shell->p_size) \
				|| fork_process(&shell->p[index]))
			break ;
		child(shell, index);
		parent(shell, index);
		index++;
	}
	wait_process(shell);
	set_signal_init(shell, handler_init);
}

void	set_fd_builtin(t_process *p)
{
	int	fd_in;
	int	fd_out;

	fd_in = dup(STDIN_FILENO);
	fd_out = dup(STDOUT_FILENO);
	if (p->redirect_fd[0] > 0)
		dup_fd(&p->redirect_fd[0], fd_in);
	if (p->redirect_fd[1] > 0)
		dup_fd(&p->redirect_fd[1], fd_out);
	p->redirect_fd[0] = fd_in;
	p->redirect_fd[1] = fd_out;
}

//free 추가해주기
void	inprocess(t_shell *shell)
{
	long	status;

	status = exec_builtin(shell->p[0], &shell->data);
	if (!ft_memcmp(shell->p[0].args[0], "exit", 5) && !status)
	{
		exit(g_status);
	}
}

int	token_to_word(t_shell *shell, t_token *t, size_t index)
{
	t_process	*p;

	if (!t || t->type != T_CMD_WORD)
		return (EXIT_SUCCESS);
	p = &shell->p[index];
	p->args = get_cmdargs(t->argvs);
	p->path = get_cmdpath(shell->data.paths, t->word);
	if (!p->args || !p->path)
		return (EXTRA_ERROR);
	return (EXIT_SUCCESS);
}

int	token_to_process(t_shell *shell, t_token *t, size_t *index)
{
	int	status;

	status = EXIT_SUCCESS;
	if (t->type == T_SIMPLE_CMD)
	{
		if (token_to_word(shell, t->right, *index) \
		|| find_redirect(&shell->p[*index], t->left))
			return (EXTRA_ERROR);
		shell->p[*index].index = *index;
		*index += 1;
		return (EXIT_SUCCESS);
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

	index = 0;
	shell->p_size = find_pipe(shell->t);
	shell->p = ft_calloc(shell->p_size + 1, sizeof(t_process));
	if (!shell->p || token_to_process(shell, shell->t, &index))
	{
		g_status = EXIT_FAILURE;
		handle_error(NULL, NULL, EXTRA_ERROR);
	}
	else if (shell->p_size == 1 && is_builtin(shell->p[0].path))
		inprocess(shell);
	else
		subprocess(shell);
}

// void print_token(t_token *t)
// {
// 	const char *d[] = {"", "|", "simple cmd", "cmd word", "redirects", ">>", "<<", ">", "<"};
// 	if (!t)
// 		return;
// 	printf("type is %s\n", d[t->type]);
// 	if (t->word)
// 		printf("token word: %s\n", t->word);
// 	print_token(t->left);
// 	if (t->right && t->right->type == T_SIMPLE_CMD)
// 		printf("\n\n\n");
// 	print_token(t->right);	
// }

// char	**debug(char *path, char **args)
// {
	// size_t	index;
	// size_t	size;
	// size_t	add;
	// char	**debug;
// 
	// index = 0;
	// size = 0;
	// add = 0;
	// while (args[size])
		// size++;
	// debug = ft_calloc(size + 2, sizeof(char *));
	// if (ft_memcmp(path, args[0], ft_strlen(path) + 1))
		// debug[add++] = ft_strdup(path);
	// while (index < size)
	// {
		// debug[index + add] = args[index];
		// index++;
	// }
	// debug[++index] = 0;
	// return (debug);
// }
