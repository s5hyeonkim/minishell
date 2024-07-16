#include "minishell.h"


/* exit */
void	exit_process(t_shell *shell, char *obj, int errcode)
{
	set_terminal_printon(shell);
	if (errcode && errcode < CMD_NOT_FOUND)
	{
		errcode = EXIT_FAILURE;
		handle_error(obj, NULL, errcode);
	}
	else if (errcode == CMD_NOT_FOUND)
		handle_error(obj, NULL, errcode);
	if (errcode == SIGEXIT + SIGTERM)
		errcode = EXIT_SUCCESS;
	free_shell(*shell);
	exit(errcode);
}

/* set */
char	**get_env_paths(char *envp[])
{
	char	**ret;
	char	*paths;
	int		size;

	size = 0;
	paths = read_val_strs(envp, "PATH");
	if (paths)
		ret = ft_split(paths, ':');
	else
		ret = ft_calloc(1, sizeof(char *));
	return (ret);
}


void	check_valid(t_shell *shell, int argc)
{
	if (argc != 1)
		exit_process(shell, NULL, INVALID_ARGV);
}



void	set_tokens(t_shell *shell, char *buffer)
{
	if (set_token(&(shell->t)))
		exit_process(shell, NULL, EXTRA_ERROR);
	shell->t->cmd = buffer;
	// printf("token start %s\n", shell->t->cmd);
	tokenization(shell, shell->t);
	// printf("token end\n");
}

/* main */

void	init(t_shell *shell, int argc, char *envp[])
{
	set_shell(shell, envp);
	check_valid(shell, argc);
	init_terminal(shell);
	set_signal_init(shell, handler_init);
}

void	readlines(t_shell *shell, char **buffer)
{
	*buffer = readline(PROMPT_MSG);
	(void) shell;
	if (*buffer == 0)
	{
		handler_init(SIGTERM);
		exit_process(shell, NULL, status);
	}
	if (**buffer)
		add_history(*buffer);
}

void	set_status(t_shell *shell)
{
	if (status)
		shell->status = status;
	status = 0;
}

void	loop(t_shell *shell)
{
	char	*buffer;

	// rl_clear_history();
	while(1)
	{
		readlines(shell, &buffer);
		// printf("buffer: %s\n", buffer);
		set_status(shell);
		set_tokens(shell, buffer);
		
		/* exec */
		set_process(shell);
		set_cmds(shell);
		exec_cmds(shell);
		free_cmds(&shell->t, &shell->p, shell->p_size);
		set_signal_init(shell, handler_init);
	}
}

// void leaks ()
// {
// 	system("leaks minishell");
// }

int	main(int argc, char *argv[], char *envp[])
{
	// atexit(leaks);
	t_shell	shell;

	(void)argv;
	init(&shell, argc, envp);
	loop(&shell);
}
