#include "minishell.h"

/* terminal */

void	init_terminal(t_shell *shell)
{
	tcgetattr(1, &shell->term);
	printf("init: %lx\n", shell->term.c_lflag);
}

void	set_terminal_printoff(void)
{
	t_termios	term;

	tcgetattr(1, &term);
	term.c_lflag &= ~(ECHOCTL);
	tcsetattr(1, 0, &term);
	// ft_putstr_fd("here\n", 2);
	// printf("off: %lx\n", term.c_lflag);
}

void	set_terminal_printon(t_shell *shell)
{
	tcsetattr(1, 0, &shell->term);
	printf("on: %lu\n", shell->term.c_lflag);
}

/* signal_execute */

void	child(t_shell *shell, int index)
{
	printf("child process start\n");
	int	ret;
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
	// set_signal_parent(shell, main_handler);
}
void	replace_line(int redisplayon);

void	handler(int signo)
{
	status = signo + SIGEXIT;
	// printf("signal receive %d\n", status);
	if (signo == SIGINT)
	{
		ft_putchar_fd('\n', STDERR_FILENO);
		
		rl_on_new_line();
		rl_replace_line("", 0);
		
		set_terminal_printoff();
	}
	if (signo == SIGTERM)
	{
		// move_cursor();
		ft_putstr_fd("main_sigterm\n", STDERR_FILENO);
		ft_putstr_fd("exit\n", STDERR_FILENO);
	}
	if (signo == SIGQUIT)
	{
		ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
	}
	// set_terminal_printoff();
}
void	child_handler(int signo) ;

void	exec_cmds(t_shell *shell)
{
	if (shell->p_size > 1 || !is_builtin(shell->p[0].path))
	{
		set_signal_child(shell, SIG_DFL);
		subprocess(shell);
		set_terminal_printon(shell);
		set_signal_default(shell, handler);
		wait_process(shell);
		// set_terminal_printoff();
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
	set_terminal_printon(shell);
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
	set_signal_default(shell, main_handler);
	init_terminal(shell);
	set_terminal_printoff();
}

void	readlines(t_shell *shell, char **buffer)
{
	*buffer = readline(PROMPT_MSG);
	(void) shell;
	if (*buffer == 0)
	{
		main_handler(SIGTERM);
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
		set_signal_default(shell, main_handler);
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
