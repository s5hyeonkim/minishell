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

int	heredoc_process(char *link, char *limiter, t_deques *deqs)
{
	int		status;
	char	*buffer;
	char	*parse;
	char	line[ARG_MAX];

	line[0] = 0;
	status = set_signal_init(handler_heredoc);
	parse = NULL;
	write_files(link, line);
	while (!status)
	{
		rl_replace_line("", 0);
		buffer = readline("> ");
		if(!buffer)
			status = EXTRA_ERROR;
		parse = replace_value(deqs, buffer);
		free(buffer);
		if (parse && !ft_memcmp(buffer, limiter, ft_strlen(limiter) + 1))
			break ;
		if (!parse)
			status = EXTRA_ERROR;
		if (parse)
		{
			ft_strlcat(line, buffer, ARG_MAX);
			ft_strlcat(line, "\n", ARG_MAX);
		}
		free(parse);
	}
	if (!status)
		status = write_files(link, line);
	free(buffer);
	return (status);
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
	return (EXIT_SUCCESS);
}
