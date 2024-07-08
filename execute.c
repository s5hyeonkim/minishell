#include "minishell.h"

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
	ret = NULL;
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
	free(temp);
	if (!ret)
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
		// printf("no custom\n");
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
int	check_quotation_flag(int *flag, int flag2)
{
	*flag = !(*flag);
	if (!flag2)
		return (TRUE);
	return (FALSE);
}

void	parsing_cmd(char *cmd)
{
	size_t		size;
	int			flag;
	int			flag1;

	size = 0;
	flag = 0;
	flag1 = 0;
	while (cmd[size])
	{
		if (cmd[size] == ' ' && !flag && !flag1)
			cmd[size] = 0;
		if (cmd[size] == '\'' && check_quotation_flag(&flag, flag1))
			cmd[size] = 0;
		if (cmd[size] == '\"' && check_quotation_flag(&flag1, flag))
			cmd[size] = 0;
		if (!flag1 && !flag && !cmd[size])
			break ;
		size++;
	}
}

char	**get_cmdargs(char *cmd)
{
	t_deques	*deq;
	char		**str;
	t_pairs		keyval;
	size_t		start;
	size_t		len;
	size_t		end;

	deq = create_deques();
	if (!deq)
		return (NULL);
	start = 0;
	end = 0;
	len = ft_strlen(cmd);
	while (start < len)
	{
		// printf("%zu\n", start);
		parsing_cmd(&cmd[start]);
		if (cmd[start] && (set_keyval(&cmd[start], &keyval) || push_back(deq, keyval)))
		{
			free_keyval(keyval);
			free_deques(&deq);
			return (NULL);
		}
		start = start + ft_strlen(&cmd[start]) + 1;
	}
	str = deqtoenvp(deq, NO);
	free_deques(&deq);
	return (str);
}

/* program */
	/* builtin */
int	exec_builtin(t_exec *info, t_process p)
{
	const char	*cmds[] = {"echo", "cd", "pwd", "export", "unset", "env", "exit"};
	int			index;

	index = 0;
	// printf("builtin?\n");
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

	//printf("external %s %s\n", p.path, p.args[0]);
	envp = deqtoenvp(info->data.envps, ENV);
	if (!envp)
		exit_process(info, NULL, MALLOC_FAILED);
	if (execve(p.path, p.args, envp) == -1)
		exit_process(info, p.args[0], CMD_NOT_FOUND);
}


size_t	find_pipe(t_token *t)
{
	size_t	pipe_num;

	if (!t)
		return (0);
	pipe_num = 0;
	if (t->type == PIPE)
		pipe_num++;
	pipe_num += find_pipe(t->left);
	pipe_num += find_pipe(t->right);
	return (pipe_num);
}

void	set_args(t_exec *info, t_process *p)
{
	// simple cmd로 수정 필요, 지금 t.cmd type은 redirection 포함한 cmd
	// printf("enter set args:%s\n", p->t.cmd);
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

void	set_cmds(t_exec *info)
{
	size_t	index;

	index = 0;
	while (index < info->size)
	{
		set_args(info, &info->p[index]);
		// printf("first commad: %s\n", info->p[index].args[0]);
		set_path(info, &info->p[index]);
		// printf("path/command: %s\n", info->p[index].path);
		index++;
	}
}

void	set_token_process(t_exec *info, t_token *t, int *index)
{
	if (!t)
		return ;
	if (t->type == SIMPLE_CMD || t->type == REDIRECT)
	{
		// printf("set SIMPLE CMD\n");
		info->p[*index].t = *t;
		(*index)++;
		return ;
	}
	else if (t->type)
	{
		if (t->left && t->left->cmd && t->left->cmd[0])
			set_token_process(info, t->left, index);
		if (t->right && t->right->cmd && t->right->cmd[0])
			set_token_process(info, t->right, index);
	}
}

void	set_process(t_exec *info)
{
	int	index;

	index = 0;
	info->size = find_pipe(info->t) + 1;
	info->p = ft_calloc(info->size, sizeof(t_process));
	if (!info->p)
		exit_process(info, NULL, MALLOC_FAILED);
	set_token_process(info, info->t, &index);
	// printf("set process\n");
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
	// printf("child process start\n");
	int	ret;

	set_signal(info, child_handler);
	if (is_builtin(info->p[index].path))
	{
		ret = exec_builtin(info, info->p[index]);
		exit_process(info, NULL, ret);
	}
	else
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
	size_t	index;

	index = 0;
	printf("execute in child process\n");
	while (index < info->size)
	{
		open_pipe(info, index);
		get_child(info, index);
		close_pipe(info, index);
		index++;
	}
}

void	inprocess(t_exec *info)
{
	int	ret;
	printf("execute in current process\n");
	// printf("%s\n", info->p[0].path);	
	ret = exec_builtin(info, info->p[0]);
	info->status = ret;
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
	if (info->size > 1 || !is_builtin(info->p[0].path))
	{
		subprocess(info);
		wait_process(info);
	}
	else
	{
		inprocess(info);
		//print_deques(info->data.envps);
	}
}


