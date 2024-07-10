#include "minishell.h"

/* execute parsing*/
char	*get_pathcmd(char **paths, char *cmd)
{
	int		index;
	char	*ret;

	index = 0;
	while (paths[index])
	{
		ret = ft_pairjoin(paths[index++],'/', cmd);
		if (!ret || !access(ret, X_OK))
			return (ret);
		free(ret);
	}
	return (ft_strdup(cmd));
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

char	*get_cmdpath(char **paths, char *cmd)
{
	int		err;
	char	*ret;

	err = EXIT_SUCCESS;
	if (is_builtin(cmd))
		ret = ft_strdup(cmd);
	else
		ret = get_pathcmd(paths, cmd);
	return (ret);
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

void	parsing_cmd(char *cmd, size_t *start, size_t *end)
{
	size_t		size;
	int			flag;
	int			flag1;

	size = *start;
	flag = 0;
	flag1 = 0;
	while (cmd[size] == ' ')
		size++;
	*start = size;
	while (cmd[size])
	{
		if (cmd[size] == '\'' && check_quotation_flag(&flag, flag1))
			size++;
		if (cmd[size] == '\"' && check_quotation_flag(&flag1, flag))
			size++;
		if (!flag1 && !flag && cmd[size] == ' ')
			break ;
		size++;
	}
	*end = size;
}

int	set_parsing_deques(t_deques **deqs, char *cmd)
{
	size_t		start;
	size_t		end;
	size_t		len;
	char		*str;

	start = 0;
	end = 0;
	len = ft_strlen(cmd);
	while (start < len)
	{
		parsing_cmd(cmd, &start, &end);
		// printf("%zu %zu\n", start, end);
		if (start >= len && end == start)
			break ;
		str = ft_substr(cmd, start, end - start);
		if (!str || push_keyval(*deqs, str))
		{
			free(str);
			free_deques(deqs);
			return (EXTRA_ERROR);
		}
		free(str);
		start = ++end;
	}
	return (EXIT_SUCCESS);
}

char	**get_cmdargs(char *cmd)
{
	t_deques	*deqs;
	char		**str;

	deqs = create_deques();
	if (!deqs || set_parsing_deques(&deqs, cmd))
		return (NULL);
	str = deqtoenvp(deqs, NO);
	free_deques(&deqs);
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
	return (find_builtin(index)(info, p));
}

	/* external */
void	exec_program(t_exec *info, t_process p)
{
	char	**envp;

	//printf("external %s %s\n", p.path, p.args[0]);
	envp = deqtoenvp(info->data.envps, ENV);
	if (!envp)
		exit_process(info, NULL, EXTRA_ERROR);
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
		exit_process(info, NULL, EXTRA_ERROR);
}

void	set_path(t_exec *info, t_process *p)
{
	p->path = get_cmdpath(info->data.paths, p->args[0]);
	if (!p->path)
		exit_process(info, NULL, EXTRA_ERROR);
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
		exit_process(info, NULL, EXTRA_ERROR);
	set_token_process(info, info->t, &index);
	// printf("set process\n");
}

void	open_pipe(t_exec *info, int index)
{
	if (info->p[index + 1].path)
	{
		if (pipe(info->p[index].fd))
			exit_process(info, NULL, EXTRA_ERROR);
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
		exit_process(info, NULL, EXTRA_ERROR);
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
	printf("execute in current process\n");
	// printf("%s\n", info->p[0].path);	
	info->status = exec_builtin(info, info->p[0]);
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
	info->status = status;
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


