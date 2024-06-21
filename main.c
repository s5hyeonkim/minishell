#include "minishell.h"

void	print_error(int code)
{
	if (code == CMD_NOT_FOUND)
	{
		ft_putstr_fd("minishell: command not found\n", 2);
		printf("%d\n", errno);
		perror("why?: ");
	}
	else if (code == INVALID_ARGV)
		ft_putstr_fd("minishell: invalid arguments\n", 2);
	else
		perror("minishell: ");
}

void	exit_process(t_exec *info, int errcode)
{
	if (errcode && errcode <= 127)
	{
		print_error(errcode);
		errcode = 1;
	}
	free(info->t);
	exit(errcode);
}

void	set_signal(t_exec *info, void(*func)(int))
{
	if (signal(SIGINT, func) == SIG_ERR)
		exit_process(info, SIG_ERROR);
	if (signal(SIGTERM, func) == SIG_ERR)
		exit_process(info, SIG_ERROR);
	if (signal(SIGQUIT, func) == SIG_ERR)
		exit_process(info, SIG_ERROR);
}

void	child_handler(int signo)
{
	//if (signo == SIGINT)
		//exit(128 + signo);
	exit(128 + signo);
}

void    handler(int signo)
{
	(void) signo;
}

void	check_validation(t_exec *info, int argc)
{
	if (argc != 1)
		exit_process(info, INVALID_ARGV);
}

char	**get_env_paths(void)
{
	char	**ret;
	char	*paths;
	int		size;

	size = 0;
	paths = getenv("PATH");
	if (paths)
		ret = ft_split(paths, ':');
	else
		ret = ft_calloc(1, sizeof(char *));
	return (ret);
}

char	**ft_strsdup(char **envp)
{
	size_t	len;
	char	**new;

	len = 0;
	while (envp[len])
		len++;
	new = ft_calloc(len + 10, sizeof(char *));
	len = 0;
	if (new)
	{
		while (envp[len])
		{
			new[len] = ft_strdup(envp[len]);
			len++;
		}

	}
	return (new);
}

void	set_info(t_exec *info, char *envp[])
{
	ft_memset(info, 0, sizeof(t_exec));
	info->data.envp = envp;
	if (info->data.envp)
		info->data.paths = get_env_paths();
	if (!info->data.paths || !info->data.envp)
		exit_process(info, MALLOC_FAILED);
}

void	set_terminal(void)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~(ICANON | ECHO);
	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void	set_terminal_print(void)
{
	struct termios	term;

	tcgetattr(STDOUT_FILENO, &term);
	term.c_lflag |= (ECHOCTL);
	tcsetattr(STDOUT_FILENO, 0, &term);
}

void	set_terminal_printoff(void)
{
	struct termios	term;

	tcgetattr(1, &term);
	term.c_lflag &= ~(ECHOCTL);
	tcsetattr(1, 0, &term);
}

void	init(t_exec *info, int argc, char *envp[])
{
	set_info(info, envp);
	check_validation(info, argc);
	//set_terminal();
	set_signal(info, handler);
}

void	readlines(t_exec *info, char **buffer)
{

	*buffer = readline("minishell$ ");
	if (*buffer == 0)
		exit_process(info, 128 + SIGTERM);
	if (**buffer)
		add_history(*buffer);
}

void	free_array(char **strs)
{
	size_t	index;

	index = 0;
	if (!strs)
		return ;
	while (strs[index])
		free(strs[index++]);
	free(strs);
}

int	tokenization(t_token *t, char *buffer)
{
	if (*buffer == 0)
		return (EXIT_SUCCESS);
	t->cmd = ft_strdup(buffer);
	if (!t->cmd)
		return (MALLOC_FAILED);
	return (tokenization(t->right, buffer + ft_strlen(t->cmd)));
}

void	set_tokens(t_exec *info, char *buffer)
{
	info->t = ft_calloc(1, sizeof(t_token));
	if (!info->t)
		exit_process(info, MALLOC_FAILED);
	if (tokenization(info->t, buffer))
		exit_process(info, MALLOC_FAILED);
}

// void	exec_token(t_token *t, t_env env)
// {
// 	if (!t)
// 		return ;
// 	exec_token(t->left, env);
// 	exec(t, env);
// 	exec_token(t->right, env);
// }

// void	exec_tokens(t_exec *info)
// {
// 	exec_token(info->t, info->env);
// }

char	*check_builtin(char *cmd, int *err)
{
	char	*ret;

	*err = EXIT_SUCCESS;
	ret = ft_strjoin(builtin, cmd);
	if (ret == NULL)
	{
		*err = MALLOC_FAILED;
		return (NULL);
	}
	if (!access(ret, X_OK))
		return (ret);
	free(ret);
	*err = EXIT_FAILURE;
	return (NULL);
}

char	*check_pathenv(char **paths, char *cmd)
{
	int		index;
	char	*ret;
	char	*temp;

	index = 0;
	temp = ft_strjoin("/", cmd);
	if (!temp)
		return (NULL);
	while (paths[index])
	{
		ret = ft_strjoin(paths[index++], temp);
		if (!ret)
		{
			free(temp);
			return (NULL);
		}
		if (!access(ret, X_OK))
			break ;
		free(ret);
		ret = NULL;
	}
	if (!ret)
		ret = ft_strdup(cmd);
	return (ret);
}

char	*get_cmdpath(char **paths, char *cmd)
{
	int		err;
	char	*ret;

	ret = check_builtin(cmd, &err);
	if (ret || err == MALLOC_FAILED)
		return (ret);
	ret = check_pathenv(paths, cmd);
	return (ret);
}

char	**fit_arrsize(char **str, int index, int *size)
{
	if (index == *size - 1)
	{
		*size += 10;
		str = ft_realloc(str, sizeof(char *) * *size);
	}
	return (str);
}

int	is_valid_quotation(size_t *start, int *open1, int open2)
{
	*open1 = !(*open1);
	if (!open2)
	{
		if (!(*open1))
			(*start)++;
		return (TRUE);
	}
	return (FALSE);
}

void	set_parsing_point(size_t *start, size_t *end, char *cmd)
{
	int	open[2];

	ft_memset(open, 0, sizeof(int) * 2);
	while (cmd[*start] == ' ')
		(*start)++;
	*end = *start;
	while (cmd[*end])
	{
		if (cmd[*end] == ' ' && !open[0] && !open[1])
			cmd[*end] = 0;
		if (cmd[*end] == '\'' && is_valid_quotation(start, &open[0], open[1]))
			cmd[*end] = 0;
		if (cmd[*end] == '\"' && is_valid_quotation(start, &open[1], open[0]))
			cmd[*end] = 0;
		if (!open[0] && !open[1] && !cmd[*end])
			break ;
		(*end)++;
	}
}

char	**get_args(char **strs, char *cmd)
{
	int		index;
	int		size;
	size_t	start;
	size_t	end;
	size_t	len;

	start = 0;
	end = 0;
	index = 0;
	size = 10;
	len = ft_strlen(cmd);
	while (strs && end < len)
	{
		set_parsing_point(&start, &end, cmd);
		if (cmd[start])
			strs[index] = ft_strdup(&cmd[start]);
		if (!strs[index++] && cmd[start])
		{
			free_array(strs);
			return (0);
		}
		start = ++end;
		strs = fit_arrsize(strs, index, &size);
	}
	return (strs);
}

char	**get_cmdargs(char *cmd)
{
	char	**ret;

	ret = ft_calloc(10, sizeof(char *));
	if (!ret)
		return (NULL);
	ret = get_args(ret, cmd);
	free(cmd);
	return (ret);
}

void	set_process(t_exec *info)
{
	info->p = ft_calloc(1, sizeof(t_process));
	if (!info->p)
		exit_process(info, MALLOC_FAILED);
	info->p->pid = fork();
	if (info->p->pid == -1)
		exit_process(info, FORK_FAILED);
	else if (!info->p->pid)
	{
		info->p->args = get_cmdargs(info->t->cmd);
		if (info->p->args)
			info->p->path = get_cmdpath(info->data.paths, info->t->cmd);
		if (!info->p->args || !info->p->path)
			exit_process(info, MALLOC_FAILED);
		printf("%s\n", info->p->path);
		if (execve(info->p->path, info->p->args, info->data.envp) == -1)
			exit_process(info, CMD_NOT_FOUND);
	}
	else
		waitpid(info->p->pid, 0, WNOHANG);
}

void	child(t_exec *info)
{
	char	*buffer;

	set_terminal_printoff();
	set_signal(info, child_handler);
	readlines(info, &buffer);
	set_tokens(info, buffer);
	set_process(info);
	//printf("reply: %s %d", buffer, *buffer);
	waitpid(info->p->pid, 0, 0);
	exit_process(info, 0);
}

void	parent(t_exec *info, pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		info->data.status = WEXITSTATUS(status);
		if (WEXITSTATUS(status) == 128 + SIGTERM)
			exit_process(info, 128 + SIGTERM);
		if (WEXITSTATUS(status) == 128 + SIGINT)
		{
			ft_putstr_fd("\n", STDERR_FILENO);
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}
	}
}

void	loop(t_exec *info)
{
	pid_t	pid;
	rl_clear_history();
	while (1)
	{
		pid = fork();
		if (pid == -1)
			exit_process(info, FORK_FAILED);
		else if (!pid)
			child(info);
		else
			parent(info, pid);
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	t_exec	info;

	(void)argv;
	init(&info, argc, envp);
	loop(&info);
}