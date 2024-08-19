/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 16:38:27 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/19 16:39:47 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

int	wait_heredoc(t_process p)
{
	int	status;
	int	s;

	g_status = 0;
	s = set_signal_init(handler_heredoc_wait);
	waitpid(p.pid, &status, 0);
	if (!s)
		s = set_signal_init(handler_init);
	if (!s && g_status)
		return (SIGNALED);
	else if (!s && WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (EXIT_FAILURE);
}

int	write_files(char *file_name, char *line)
{
	int	fd;

	fd = open(file_name, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	if (fd == -1)
		return (EXTRA_ERROR);
	if (line[0])
		write(fd, line, ft_strlen(line));
	close(fd);
	return (EXIT_SUCCESS);
}

int	heredoc_process(char *link, char *limiter)
{
	int		status;
	char	*buffer;
	char	line[ARG_MAX];

	line[0] = 0;
	status = set_signal_init(handler_heredoc);
	write_files(link, line);
	rl_replace_line("", 0);
	buffer = readline("> ");
	while (buffer && ft_memcmp(buffer, limiter, ft_strlen(limiter) + 1))
	{
		ft_strlcat(line, buffer, ARG_MAX);
		ft_strlcat(line, "\n", ARG_MAX);
		free(buffer);
		rl_replace_line("", 0);
		buffer = readline("> ");
	}
	status = write_files(link, line);
	free(buffer);
	return (status);
}

//
int	here_doc(char *link, char *limiter)
{
	t_process	p;
	int			status;

	if (fork_process(&p))
		return (EXTRA_ERROR);
	if (!p.pid)
	{
		status = heredoc_process(link, limiter);
		exit(status);
	}
	else
		status = wait_heredoc(p);
	return (status);
}

// 쓰는것
// free 및 close 함수에 unlink 추가 필요
//close_fd print_error 뒤에
int	set_filedoc(t_process *p)
{
	char	*itoa;

	if (!p->link)
	{
		itoa = ft_itoa(p->index);
		if (itoa)
			p->link = ft_strjoin("here_doc", itoa);
		if (!itoa || !p->link)
		{
			free(itoa);
			return (EXTRA_ERROR);
		}
		free(itoa);
	}
	return (EXIT_SUCCESS);
}
