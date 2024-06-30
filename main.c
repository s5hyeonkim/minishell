#include "minishell.h"

volatile int	status;

void	init(t_exec *info, int argc, char *envp[]);
void	set_info(t_exec *info, char **envp);
void	check_valid(t_exec *info, int argc);
void	exec_external(t_exec *info, t_process p);

/* exit */

void	free_token(t_token *t)
{
	if (t)
	{
		free_token(t->left);
		free_token(t->right);
		//free(t->cmd);
		free(t);
	}
}

void	free_data(t_data d)
{
	free_strs(d.paths);
	free_deques(&d.envps);
	// add more?
}

void	free_tprocess(t_process *p)
{
	int	index;

	index = 0;
	if (!p)
		return ;
	while (p[index].args)
	{
		free_strs(p[index].args);
		free(p[index].path);
		index++;
	}
}

void	free_info(t_exec info)
{
	free_token(info.t);
	free_data(info.data);
	free_tprocess(info.p);
}

void	exit_process(t_exec *info, char *obj, int errcode)
{
	if (errcode && errcode < 127)
	{
		print_msg(obj, errcode);
		errcode = 1;
	}
	if (errcode == 128 + SIGTERM)
	{
		ft_putstr_fd("exit\n", 2);
		errcode = 0;
	}
	free_info(*info);
	exit(errcode);
}

/* set */
char	**get_env_paths(char *envp[])
{
	char	**ret;
	char	*paths;
	int		size;

	size = 0;
	paths = get_value(envp, "PATH");
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
	info->data.envps = strstodeq(envp);
	if (info->data.envps)
		info->data.paths = get_env_paths(envp);
	if (!info->data.paths || !info->data.envps)
		exit_process(info, NULL, MALLOC_FAILED);
}

void	check_valid(t_exec *info, int argc)
{
	if (argc != 1)
		exit_process(info, NULL, INVALID_ARGV);
}

/* signal*/
void	replace_lines(void)
{
	if (status >= 130)
	{
		ft_putchar_fd('\n', STDERR_FILENO);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void    child_handler(int signo)
{
	(void) signo;
	status = signo + 128;
	//printf("signal receive %d\n", status);
	if (signo == SIGINT)
		exit(status);
}

void    handler(int signo)
{
	(void) signo;
	status = signo + 128;
	//printf("signal receive %d\n", status);
	if (signo == SIGINT)
		replace_lines();
}

void	set_signal(t_exec *info, void(*func)(int))
{
	if (signal(SIGINT, func) == SIG_ERR)
		exit_process(info, NULL, SIG_ERROR);
	if (signal(SIGTERM, func) == SIG_ERR)
		exit_process(info, NULL, SIG_ERROR);
	if (signal(SIGQUIT, func) == SIG_ERR)
		exit_process(info, NULL, SIG_ERROR);
}

/* execute parsing*/
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
	}
	if (!paths[index])
		ret = ft_strdup(cmd);
	return (ret);
}

int	is_builtin(char *cmd)
{
	const char	*cmds[] = {"echo", "cd", "pwd", "export", "unset", "env", "exit"};
	size_t		len;
	int			index;

	index = 0;
	while (index < 7)
	{
		len = ft_strlen(cmds[index]);
		if (!ft_memcmp(cmds[index], cmd, len) && (!cmd[len] || cmd[len] == ' '))
			return (TRUE);
		index++;
	}
	return (FALSE);
}

char	*check_custom(char *cmd, int *err)
{
	char	*ret;

	if (is_builtin(cmd))
	{
		ret = ft_strdup(cmd);
		if (!ret)
			*err = MALLOC_FAILED;
		return (ret);
	}
	else
	{
		ret = ft_strjoin(external, cmd);
		if (ret == NULL)
		{
			*err = MALLOC_FAILED;
			return (NULL);
		}
	}
	if (!access(ret, X_OK))
		return (ret);
	free(ret);
	*err = EXIT_FAILURE;
	return (NULL);
}

char	*get_cmdpath(char **paths, char *cmd)
{
	int		err;
	char	*ret;

	err = EXIT_SUCCESS;
	//printf("is custom?\n");
	ret = check_custom(cmd, &err);
	if (err == EXIT_FAILURE)
	{
		printf("no custom\n");
		ret = check_pathenv(paths, cmd);
	}
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

// redirection 추가 필요
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
			free_strs(strs);
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

/* program */
	/* builtin */
int	exec_builtin(t_exec *info, t_process p)
{
	const char	*cmds[] = {"echo", "cd", "pwd", "export", "unset", "env", "exit"};
	int			index;

	index = 0;
	while (index < 7)
	{
		if (!ft_memcmp(cmds[index], p.path, ft_strlen(cmds[index]) + 1))
			break ;
		index++;
	}
	return (select_builtin(index)(info, p));
}
	/* external */
void	exec_program(t_exec *info, t_process p)
{
	char	**envp;

	envp = deqtoenvp(info->data.envps);
	if (!envp)
		exit_process(info, NULL, MALLOC_FAILED);
	if (execve(p.path, p.args, envp) == -1)
		exit_process(info, p.args[0], CMD_NOT_FOUND);
}

/* parsing and set tokens 수정 필요 */
int	tokenization(t_token *t, char *buffer)
{
	if (*buffer == 0)
		return (EXIT_SUCCESS);
	t->type = CMD;
	t->cmd = ft_strdup(buffer);
	if (!t->cmd)
		return (MALLOC_FAILED);
	return (tokenization(t->right, buffer + ft_strlen(t->cmd)));
}

void	set_tokens(t_exec *info, char *buffer)
{
	info->t = ft_calloc(1, sizeof(t_token));
	if (!info->t)
		exit_process(info, NULL, MALLOC_FAILED);
	if (tokenization(info->t, buffer))
		exit_process(info, NULL, MALLOC_FAILED);
}

/* main */
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
	check_valid(info, argc);
	set_signal(info, handler);
	set_terminal_printoff();
}

void	readlines(t_exec *info, char **buffer)
{

	*buffer = readline("minishell$ ");
	if (*buffer == 0)
	{
		exit_process(info, NULL, SIGTERM + 128);
	}
	if (**buffer)
		add_history(*buffer);
}

int	find_pipe(t_token *t)
{
	int	pipe_num;

	if (!t)
		return (0);
	pipe_num = 0;
	if (t->type == PIPE)
		pipe_num++;
	pipe_num += find_pipe(t->left);
	pipe_num += find_pipe(t->right);
	return (pipe_num);
}

void	find_cmd(t_exec *info, t_token *t, int *idx)
{
	if (!t)
		return ;
	find_cmd(info, t->left, idx);
	if (t->type == CMD)
	{
		info->p[*idx].t = *t;
		*idx += 1;
	}
	find_cmd(info, t->right, idx);
}

void	set_args(t_exec *info, t_process *p)
{
	// simple cmd로 수정 필요, 지금 t.cmd type은 redirection 포함한 cmd
	p->args = get_cmdargs(p->t.cmd);
	if (!p->args)
		exit_process(info, NULL, MALLOC_FAILED);
}

void	set_path(t_exec *info, t_process *p)
{
	p->path = get_cmdpath(info->data.paths, p->args[0]);
	if (!p->path)
		exit_process(info, NULL, MALLOC_FAILED);
}

void	set_cmd(t_exec *info)
{
	int	index;

	index = 0;
	find_cmd(info, info->t, &index);
	printf("number of command: %d\n", index);
	while (index--)
	{
		set_args(info, &info->p[index]);
		//printf("first commad: %s\n", info->p[index].args[0]);
		set_path(info, &info->p[index]);
		printf("path/command: %s\n", info->p[index].path);
	}
}

void	set_cmds(t_exec *info)
{
	int	pipenum;

	pipenum = 0;
	pipenum += find_pipe(info->t);
	info->p = ft_calloc(pipenum + 2, sizeof(t_token));
	if (!info->p)
		exit_process(info, NULL, MALLOC_FAILED);
	set_cmd(info);
}

void	open_pipe(t_exec *info, int index)
{
	if (info->p[index + 1].path)
	{
		if (pipe(info->p[index].fd))
			exit_process(info, NULL, PIPE_FAILED);
	}
}

void	close_pipe(t_exec *info, int index)
{
	if (info->p[index + 1].path)
		close(info->p[index].fd[1]);
}

void	child(t_exec *info, int index)
{
	set_signal(info, child_handler);
	exec_builtin(info, info->p[index]);
	exec_program(info, info->p[index]);
}

void	parent(t_exec *info, int index)
{
	close_pipe(info, index);
	waitpid(info->p[index].pid, 0, WNOHANG);
}

void	get_child(t_exec *info, int index)
{
	info->p[index].pid = fork();
	if (info->p[index].pid == -1)
		exit_process(info, NULL, FORK_FAILED);
	else if (!info->p[index].pid)
		child(info, index);
	else
		parent(info, index);
}

void	subprocess(t_exec *info)
{
	int			index;

	index = 0;
	printf("execute in child process\n");
	while (info->p[index].path)
	{
		open_pipe(info, index);
		get_child(info, index);
		close_pipe(info, index);
		index++;
	}
}

void	inprocess(t_exec *info)
{
	printf("execute in current process\n");
	exec_builtin(info, info->p[0]);
	printf("%s\n", info->p[0].path);	
}

void	wait_process(t_exec *info)
{
	int	index;
	int	status;

	index = 0;
	while (info->p[index].path)
	{
		waitpid(info->p[index].pid, &status, 0);
		if (WIFEXITED(status))
			status = WEXITSTATUS(status);
		index++;
	}
}

void	exec_cmds(t_exec *info)
{
	if (info->p[1].path || !is_builtin(info->p[0].path))
	{
		subprocess(info);
		wait_process(info);
	}
	else
		inprocess(info);
}


void	loop(t_exec *info)
{
	char	*buffer;

	rl_clear_history();
	while(1)
	{
		readlines(info, &buffer);
		set_tokens(info, buffer);
		set_cmds(info);
		exec_cmds(info);
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	t_exec	info;

	(void)argv;
	init(&info, argc, envp);
	loop(&info);
}
