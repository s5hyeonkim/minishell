#include "minishell.h"

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

	if (fork_process(&p))
		return (EXIT_FAILURE);
	if (!p.pid)
		exit(heredoc_process(link, limiter));
	else
		return (wait_heredoc(p));
	
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
