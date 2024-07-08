#include "minishell.h"

volatile int	status;
/* exit */

void	exit_process(t_exec *info, char *obj, int errcode)
{
	if (errcode && errcode <= 127)
	{
		print_msg(obj, errcode);
		if (errcode != 127)
			errcode = 1;
	}
	if (errcode == 128 + SIGTERM)
	{
		ft_putstr_fd("exit\n", 2);
		errcode = 0;
	}
	(void) info;
	//free_info(*info);
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
	ft_putchar_fd('\n', STDERR_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();

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

/* parsing and set tokens 수정 필요 */
void	tokenization(t_exec *info, t_token *t)
{
	printf("origin cmd: %s\n", t->cmd);
	t->type = SIMPLE_CMD;
	(void) info;
	//tokenization(info, t->left);
	//tokenization(info, t->right);
}

int	set_token(t_token **t)
{
	*t = ft_calloc(1, sizeof(t_token));
	if (*t == NULL)
		return (MALLOC_FAILED);
	return (EXIT_SUCCESS);
}

void	set_tokens(t_exec *info, char *buffer)
{
	if (set_token(&(info->t)))
		exit_process(info, NULL, MALLOC_FAILED);
	info->t->cmd = buffer;
	// printf("token start %s\n", info->t->cmd);
	tokenization(info, info->t);
	// printf("token end\n");
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
		exit_process(info, NULL, SIGTERM + 128);
	if (**buffer)
		add_history(*buffer);
}

void	set_status(t_exec *info)
{
	if (status)
		info->status = status;
	status = 0;
}

void	loop(t_exec *info)
{
	char	*buffer;

	rl_clear_history();
	while(1)
	{
		readlines(info, &buffer);
		// printf("buffer: %s\n", buffer);
		set_status(info);
		set_tokens(info, buffer);
		set_process(info);
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
