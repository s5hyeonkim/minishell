#include "minishell.h"

void	clean_cmds(t_shell *shell)
{
	size_t	index;

	index = 0;
	while (index < shell->p_size)
	{
		if (shell->p[index].redirect_fd[0] > 2)
		{
			close(shell->p[index].redirect_fd[0]);
			shell->p[index].redirect_fd[0] = 0;
		}
		if (shell->p[index].redirect_fd[1] > 2)
		{
			close(shell->p[index].redirect_fd[1]);
			shell->p[index].redirect_fd[1] = 0;
		}
		if (shell->p[index].pipe_fd[0] > 2)
		{
			close(shell->p[index].pipe_fd[0]);
			shell->p[index].pipe_fd[0] = 0;
		}
		if (shell->p[index].pipe_fd[1] > 2)
		{
			close(shell->p[index].pipe_fd[1]);
			shell->p[index].pipe_fd[1] = 0;
		}
		index++;
	}
	free_cmds(&shell->t, &shell->p, &shell->p_size);
}

/* token lst */
int set_token(t_token **t)
{
	*t = ft_calloc(1, sizeof(t_token));
	if (*t == NULL)
		return (EXTRA_ERROR);
	return (EXIT_SUCCESS);
}

/* exit */
void	exit_process(t_shell *shell, char *obj, int errcode)
{
	terminal_printon(shell);
	if (errcode && errcode < CMD_NOT_FOUND)
	{
		errcode = EXIT_FAILURE;
		handle_error(obj, NULL, errcode);
	}
	else if (errcode == CMD_NOT_FOUND)
		handle_error(obj, NULL, errcode);
	if (errcode == SIGEXIT + SIGTERM)
		errcode = EXIT_SUCCESS;
	clean_cmds(shell);
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
		shell->data.status = status;
	status = 0;
}


void free_tokens(t_token *tokens)
{
	t_token *left;
	t_token *right;
	
	if (!tokens)
		return ;
	if (tokens->word)
		free(tokens->word);
	if (tokens->argvs)
		free_strs(tokens->argvs);
	left = tokens->left;
	right = tokens->right;
	free(tokens);
	free_tokens(left);
	free_tokens(right);
}

void	loop(t_shell *shell)
{
	char	*buffer;

	// rl_clear_history();
	while(1)
	{
		readlines(shell, &buffer);
		printf("buffer: %s\n", buffer);
		set_status(shell);
		parselines(shell, buffer); // parseline에서 malloc실패 등 에러나면 continue 분기문 만들어주기
		// printf("==loop==\n");
		// print_tree(shell->t, 2, 0);
		printf("========\n");
		exec_cmds(shell);
		clean_cmds(shell);
	}
}

void leaks ()
{
	system("leaks minishell");
}

int	main(int argc, char *argv[], char *envp[])
{
	// atexit(leaks);
	t_shell	shell;

	(void)argv;
	init(&shell, argc, envp);
	loop(&shell);
}
