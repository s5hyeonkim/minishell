#include "minishell.h"

void	wait_process(t_shell *shell)
{
	int	index;
	int	status;

	index = 0;
	while (shell->p[index].path)
	{
		waitpid(shell->p[index].pid, &status, 0);
		if (WIFEXITED(status))
			shell->data.status = WEXITSTATUS(status);
		index++;
	}
}

void	subprocess(t_shell *shell)
{
	size_t	index;

	index = 0;
	// printf("execute in child process\n");
	set_signal_sub(shell, handler_sub); 
	while (index < shell->p_size)
	{
		open_pipe(shell, index);
		child(shell, index);
		parent(shell, index);
		index++;
	}
	wait_process(shell);
	set_signal_init(shell, handler_init);
}

void	set_fd_builtin(t_process *p)
{
	int	fd_in;
	int	fd_out;

	fd_in = dup(STDOUT_FILENO);
	fd_out = dup(STDIN_FILENO);
	if (p->redirect_fd[0] > 0)
		dup_fd(&p->redirect_fd[0], fd_in);
	if (p->redirect_fd[1] > 0)
		dup_fd(&p->redirect_fd[1], fd_out);
	p->redirect_fd[0] = fd_in;
	p->redirect_fd[1] = fd_out;
}

void	inprocess(t_shell *shell)
{
	long	status;

	printf("execute in current process\n");
	// printf("%s\n", shell->p[0].path);
	// for (int i = 0; shell->p[0].args[i]; i++)
		// printf("%s\n", shell->p[0].args[i]);
	set_fd_builtin(shell->p);
	status = exec_builtin(shell->p[0], &shell->data);
	if (!ft_memcmp(shell->p[0].args[0], "exit", 5) && !status)
	{
		status = shell->data.status;
        clean_cmds(shell);
		exit(status);
	}
	shell->data.status = status;
}

char	**debug(char *path, char **args)
{
	size_t	index;
	size_t	size;
	size_t	add;
	char	**debug;

	index = 0;
	size = 0;
	add = 0;
	while (args[size])
		size++;
	debug = ft_calloc(size + 2 , sizeof(char *));
	if (ft_memcmp(path, args[0], ft_strlen(path) + 1))
		debug[add++] = ft_strdup(path);
	while (index < size)
	{
		debug[index + add] = args[index];
		index++;
	}
	debug[++index] = 0;
	return debug;
}


int   token_to_process(t_shell *shell, t_token *t, size_t *index)
{
    int         status;
    t_process   *p;

    p = &shell->p[*index];
    status = EXIT_SUCCESS;
    if (t->type == T_SIMPLE_CMD)
    {
		
        p->args = get_cmdargs(t->right->argv);
        if (p->args)
            p->path = get_cmdpath(shell->data.paths, t->right->word);
		// p->args = debug(t->right->word, p->args);
		// for (int i = 0; shell->p[*index].args[i] != 0; i++)
			// printf("i: %d args %s\n", i, shell->p[*index].args[i]);
		// printf("path %s\n", shell->p[*index].path);
        if (!p->args || !p->path || open_redirect(p, t->left) == -1)
            return (EXTRA_ERROR);
        return (EXIT_SUCCESS);
    }
    if (t->left)
        status = token_to_process(shell, t->left, index);
    if (!status && t->right)
        status = token_to_process(shell, t->right, index);
	// printf("ok ttop %d\n", status);
    return (status);
}

void    exec_cmds(t_shell *shell)
{
    size_t  index;

    index = 0;
    shell->p_size = find_pipe(shell->t) + 1;
    shell->p = ft_calloc(shell->p_size + 1, sizeof(t_process));
    if (!shell->p || token_to_process(shell, shell->t, &index))
    {
        shell->data.status = EXIT_FAILURE;
        handle_error("", "", EXTRA_ERROR);
    }
    else if (shell->p_size == 1 && is_builtin(shell->p[0].path))
		inprocess(shell);
	else
		subprocess(shell);
}
