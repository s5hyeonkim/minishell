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

void	inprocess(t_shell *shell)
{
	long	status;

	// printf("execute in current process\n");
	// printf("%s\n", shell->p[0].path);
	status = exec_builtin(shell->p[0], &shell->data);
	if (!ft_memcmp(shell->p[0].args[0], "exit", 5) && !status)
	{
		status = shell->data.status;
        clean_cmds(shell);
		exit(status);
	}
	shell->data.status = status;
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
		// for (int i = 0; shell->p[*index].args[i] != 0; i++)
			// printf("args %s\n", shell->p[*index].args[i]);
		// printf("path %s\n", shell->p[*index].path);
        if (!p->args || !p->path || (t->left && open_redirect(p, t->left)))
            return (EXTRA_ERROR);
        return (EXIT_SUCCESS);
    }
    if (t->left)
        status = token_to_process(shell, t->left, index);
    if (!status && t->right)
        status = token_to_process(shell, t->right, index);
    return (status);
}

void    exec_cmds(t_shell *shell)
{
    size_t  index;

    index = 0;
    shell->p_size = find_pipe(shell->t);
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
