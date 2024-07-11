#include "minishell.h"

/* exit */

void	exit_process(t_shell *shell, char *obj, int errcode)
{
	free_shell(*shell);
	if (errcode && errcode <= 127)
		exit(handle_error(obj, NULL, errcode));
	if (errcode == SIGEXIT + SIGTERM)
	{
		move_cursor();
		ft_putstr_fd("exit\n", STDERR_FILENO);
		errcode = 0;
	}
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

void	set_data(t_shell *shell, char *envp[])
{
	shell->data.envps = strstodeq(envp);
	shell->data.paths = get_env_paths(envp);
	if (shell->data.envps)
	{
		shell->data.home = ft_strdup(read_val_deq(shell->data.envps, "HOME"));
		shell->data.user = ft_strdup(read_val_deq(shell->data.envps, "USER"));
	}
	if (!shell->data.paths || !shell->data.envps || !shell->data.home || !shell->data.user)
		exit_process(shell, NULL, EXTRA_ERROR);
}

void	set_shell(t_shell *shell, char *envp[])
{
	ft_memset(shell, 0, sizeof(t_shell));
	set_data(shell, envp);
}

void	check_valid(t_shell *shell, int argc)
{
	if (argc != 1)
		exit_process(shell, NULL, INVALID_ARGV);
}

/* parsing and set tokens 수정 필요 */
void	tokenization(t_shell *shell, t_token *t)
{
	printf("origin cmd: %s\n", t->cmd);
	t->type = SIMPLE_CMD;
	(void) shell;
	//tokenization(shell, t->left);
	//tokenization(shell, t->right);
}

int	set_token(t_token **t)
{
	*t = ft_calloc(1, sizeof(t_token));
	if (*t == NULL)
		return (EXTRA_ERROR);
	return (EXIT_SUCCESS);
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
	set_signal(shell, signal_handler);
	set_terminal_printoff();
}

void	readlines(t_shell *shell, char **buffer)
{
	*buffer = readline(PROMPT_MSG);
	if (*buffer == 0)
		exit_process(shell, NULL, SIGTERM + SIGEXIT);
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

	rl_clear_history();
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
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	t_shell	shell;

	(void)argv;
	init(&shell, argc, envp);
	loop(&shell);
}
