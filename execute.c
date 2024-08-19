/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 07:26:26 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/19 16:42:37 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	exit_subprocess(t_shell *shell, char *obj, int errcode)
{
	terminal_printon();
	if (errcode && errcode < CMD_NOT_FOUND)
	{
		errcode = EXIT_FAILURE;
		handle_error(obj, NULL, errcode);
	}
	else if (errcode == CMD_NOT_FOUND)
		handle_error(obj, NULL, errcode);
	if (errcode == SIGEXIT + SIGTERM)
		errcode = EXIT_SUCCESS;
	free_shell(*shell);
	exit(errcode);
}

void	exec_program(t_shell *shell, t_process p)
{
	char	**envp;

	envp = deqtostrs(shell->data.envps->head);
	if (!envp)
		exit_subprocess(shell, NULL, EXTRA_ERROR);
	if (execve(p.path, p.args, envp) == -1)
		exit_subprocess(shell, p.args[0], CMD_NOT_FOUND);
}

// child free
void	child(t_shell *shell, size_t index)
{
	int	ret;

	if (shell->p[index].pid)
		return ;
	close_pipe(shell, index);
	set_fd(shell, index);
	if (is_builtin(shell->p[index].path))
	{
		ret = exec_builtin(shell->p[index], &shell->data);
		if (!ft_memcmp(shell->p[index].path, "exit", 5) && !ret)
			exit_wo_error(shell, g_status);
		g_status = ret;
	}
	else if (!shell->p[index].args[0][0])
		exit_wo_error(shell, g_status);
	else
	{
		exec_program(shell, shell->p[index]);
	}
}

void	parent(t_shell *shell, size_t index)
{
	if (!shell->p[index].pid)
		return ;
	close_pipe(shell, index);
	waitpid(shell->p[index].pid, 0, WNOHANG);
}
