#include "minishell.h"

volatile int	status;
/* exit */

void	exit_process(t_exec *info, char *obj, int errcode)
{
	free_info(*info);
	if (errcode && errcode <= 127)
		exit(handle_error(obj, NULL, errcode));
	if (errcode == 128 + SIGTERM)
	{
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

void	set_data(t_exec *info, char *envp[])
{
	info->data.envps = strstodeq(envp);
	info->data.paths = get_env_paths(envp);
	if (info->data.envps)
	{
		info->data.home = ft_strdup(read_val_deq(info->data.envps, "HOME"));
		info->data.user = ft_strdup(read_val_deq(info->data.envps, "USER"));
	}
	if (!info->data.paths || !info->data.envps || !info->data.home || !info->data.user)
		exit_process(info, NULL, EXTRA_ERROR);
}

void	set_info(t_exec *info, char *envp[])
{
	ft_memset(info, 0, sizeof(t_exec));
	set_data(info, envp);
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
		exit_process(info, NULL, EXTRA_ERROR);
	if (signal(SIGTERM, func) == SIG_ERR)
		exit_process(info, NULL, EXTRA_ERROR);
	if (signal(SIGQUIT, func) == SIG_ERR)
		exit_process(info, NULL, EXTRA_ERROR);
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
		return (EXTRA_ERROR);
	return (EXIT_SUCCESS);
}

void	set_tokens(t_exec *info, char *buffer)
{
	if (set_token(&(info->t)))
		exit_process(info, NULL, EXTRA_ERROR);
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
