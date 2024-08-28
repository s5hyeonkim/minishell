/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 16:38:27 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/21 17:17:13 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

int	wait_heredoc(t_process p)
{
	int	status;
	int	s;
	int	ret;

	g_status = 0;
	s = set_signal_init(handler_heredoc_wait);
	ret = -1;
	while (ret == -1)
		ret = waitpid(p.pid, &status, 0);
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

int	heredoc_process(char *link, char *limiter, t_deques *deqs)
{
	char	*buffer;
	char	line[ARG_MAX];

	line[0] = 0;
	if (set_signal_init(handler_heredoc))
		return (EXTRA_ERROR);
	write_files(link, line);
	while (TRUE)
	{
		rl_replace_line("", 0);
		buffer = readline("> ");
		if (!buffer)
			break ;
		buffer = replace_value(deqs, buffer);
		if (!buffer)
			return (EXTRA_ERROR);
		if (!ft_memcmp(buffer, limiter, ft_strlen(limiter) + 1))
			break ;
		ft_strlcat(line, buffer, ARG_MAX);
		ft_strlcat(line, "\n", ARG_MAX);
		free(buffer);
	}
	rl_replace_line("", 0);
	free(buffer);
	return (write_files(link, line));
}

int	here_doc(char *link, char *limiter, t_deques *envps)
{
	t_process	p;
	int			status;

	if (fork_process(&p))
		return (EXTRA_ERROR);
	if (!p.pid)
	{
		status = heredoc_process(link, limiter, envps);
		exit(status);
	}
	else
		status = wait_heredoc(p);
	return (status);
}

int	set_filedoc(t_process *p)
{
	char	*itoa;

	if (!p->file.link)
	{
		itoa = ft_itoa(p->index);
		if (!itoa)
			return (EXTRA_ERROR);
		p->file.link = ft_strjoin("here_doc", itoa);
		free(itoa);
		if (!p->file.link)
			return (EXTRA_ERROR);
	}
	p->file.flag = 1;
	return (EXIT_SUCCESS);
}
