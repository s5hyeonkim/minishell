/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   next_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 16:25:15 by yubshin           #+#    #+#             */
/*   Updated: 2024/08/21 11:49:35 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		read_next_cmd(int fd);
int		wait_reading(t_process p, char **s);
void	handler_heredoc1(int signo);
void	handler_heredoc_wait1(int signo);

int	set_next_cmd(char **s)
{
	t_process	p;

	if (pipe(p.pipe_fd) == -1 || fork_process(&p))
		return (EXTRA_ERROR);
	if (!p.pid)
	{
		close(p.pipe_fd[0]);
		exit(read_next_cmd(p.pipe_fd[1]));
	}
	else
	{
		close(p.pipe_fd[1]);
		return (wait_reading(p, s));
	}
}

int	read_next_cmd(int fd)
{
	int		status;
	char	*buffer;

	status = set_signal_init(handler_heredoc1);
	rl_replace_line("", 0);
	buffer = readline("> ");
	while (*buffer == 0)
	{
		free(buffer);
		buffer = readline("> ");
	}
	write(fd, buffer, ft_strlen(buffer));
	free(buffer);
	return (status);
}

int	wait_reading(t_process p, char **s)
{
	int		status;
	char	buffer[ARG_MAX];
	ssize_t	byte;

	set_signal_init(handler_heredoc_wait1);
	byte = read(p.pipe_fd[0], buffer, sizeof(buffer) - 1);
	if (byte > 0)
	{
		buffer[byte] = 0;
		printf("%s\n", buffer);
	}
	waitpid(p.pid, &status, 0);
	*s = ft_strdup(buffer);
	set_signal_init(handler_sub);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (EXTRA_ERROR);
}

void	handler_heredoc1(int signo)
{
	if (signo == SIGINT)
	{
		rl_replace_line("", 0);
		exit(1);
	}
}

void	handler_heredoc_wait1(int signo)
{
	if (signo == SIGINT)
		replace_line(FALSE);
}
