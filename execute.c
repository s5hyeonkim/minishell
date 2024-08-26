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
		handle_error(obj, NULL, errcode);
		errcode = EXIT_FAILURE;
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
	char	*pr;
	int		status;

	envp = deqtostrs(shell->data.envps->head);
	if (!envp)
		exit_subprocess(shell, NULL, EXTRA_ERROR);
	if (execve(p.exec.path, p.exec.args, envp) == -1)
	{
		if (!ft_strchr(p.exec.args[0], '/'))
			exit_subprocess(shell, p.exec.args[0], CMD_NOT_FOUND);
		status = EXTRA_ERROR;
		pr = get_nextdir(p.exec.path, shell->data.lcwd);
		if (pr && is_folder(pr))
		{
			free(pr);
			exit_subprocess(shell, p.exec.args[0], E_ISDIR);
		}
		if (pr)
		{
			if (!access(pr, X_OK))
				execve(pr, p.exec.args, envp);
		}
		exit_subprocess(shell, p.exec.args[0], status);
	}
}

void	child(t_shell *shell, size_t index)
{
	int	ret;

	if (shell->p[index].pid)
		return ;
	close_pipe(shell, index);
	set_fd(shell, index);
	if (is_builtin(shell->p[index].exec.path))
	{
		ret = exec_builtin(shell->p[index], &shell->data);
		if (!ft_memcmp(shell->p[index].exec.path, "exit", 5) && !ret)
			exit_wo_error(shell, g_status);
		exit_subprocess(shell, shell->p[index].exec.args[0], ret);
	}
	else if (!shell->p[index].exec.args[0][0])
		exit_wo_error(shell, g_status);
	else
		exec_program(shell, shell->p[index]);
}

void	parent(t_shell *shell, size_t index)
{
	if (!shell->p[index].pid)
		return ;
	close_pipe(shell, index);
	waitpid(shell->p[index].pid, 0, WNOHANG);
}
