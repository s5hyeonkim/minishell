#include "minishell.h"

/* signal_execute */

void	child(t_shell *shell, int index)
{
	// printf("child process start\n");
	int	ret;

	set_signal(shell, child_handler);
	if (is_builtin(shell->p[index].path))
	{
		ret = exec_builtin(shell, shell->p[index]);
		exit_process(shell, NULL, ret);
	}
	else
		exec_program(shell, shell->p[index]);
}

void	parent(t_shell *shell, int index)
{
	int child_status;

	// printf("status:%d\n shell->status:%d\n", status, shell->status);
	close_pipe(shell, index);
	waitpid(shell->p[index].pid, &child_status, WNOHANG);
	signal(SIGINT, SIG_IGN);
}

void	exec_cmds(t_shell *shell)
{
	if (shell->p_size > 1 || !is_builtin(shell->p[0].path))
	{
		subprocess(shell);
		wait_process(shell);
		set_signal(shell, main_handler);
	}
	else
	{
		inprocess(shell);
		//print_deques(shell->data.envps);
	}
}


/* exit */

void	exit_process(t_shell *shell, char *obj, int errcode)
{
	free_shell(*shell);
	if (errcode && errcode <= 127)
		exit(handle_error(obj, NULL, errcode));
	if (errcode == SIGEXIT + SIGTERM)
	{
		// move_cursor();
		// ft_putstr_fd("exit\n", STDERR_FILENO);
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
	set_signal(shell, main_handler);
	set_terminal_printoff();
}

void	readlines(t_shell *shell, char **buffer)
{
	*buffer = readline(PROMPT_MSG);
	// (void) shell;
	if (*buffer == 0)
		// return ;
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
		// printf("signal receive %d\n", status);
		readlines(shell, &buffer);
		// if (!buffer)
		// 	continue;
		// printf("buffer: %s\n", buffer);
		set_status(shell);
		set_tokens(shell, buffer);
		
		/* exec */
		set_process(shell);
		set_cmds(shell);
		exec_cmds(shell);
	}
}

void leaks ()
{
	system("leaks minishell");
}


int	main(int argc, char *argv[], char *envp[])
{
	atexit(leaks);
	t_shell	shell;

	(void)argv;
	init(&shell, argc, envp);
	loop(&shell);
}
