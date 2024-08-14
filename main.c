#include "minishell.h"


/* exit */
void	exit_process(t_shell *shell, char *obj, int errcode)
{
	terminal_printon();
	if (errcode && errcode < CMD_NOT_FOUND)
	{
		errcode = EXIT_FAILURE;
		handle_error(obj, NULL, errcode);
	}
	else if (errcode == CMD_NOT_FOUND)
		handle_error(obj, NULL, errcode);
	if (errcode == SIGEXIT + SIGTERM)
		errcode = EXIT_SUCCESS;
	clean_files(shell->p, shell->p_size);
	reset_terminal(shell);
	free_shell(*shell);
	exit(errcode);
}

/* set */
char	**get_env_paths(char *envp[])
{
	char	**ret;
	char	*paths;

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
	get_terminal(shell);
	if (set_signal_init(handler_init))
		exit_process(shell, NULL, EXTRA_ERROR);
}

void	readlines(t_shell *shell, char **buffer)
{
	*buffer = readline(PROMPT_MSG);
	(void) shell;
	if (*buffer == 0)
	{
		handler_init(SIGTERM);
		exit_process(shell, NULL, g_status);
	}
	if (**buffer)
		add_history(*buffer);
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
		if (parselines(shell, buffer) == EXIT_FAILURE)
			continue;; // parseline에서 malloc실패 등 에러나면 continue 분기문 만들어주기
		// printf("==loop==\n");
		// print_tree(shell->t, 2, 0);
		// printf("==loop complete==\n");
		exec_cmds(shell);
		clean_files(shell->p, shell->p_size);
		clean_process(shell->p, shell->p_size);
		rl_replace_line("", 0);
		shell->p = 0;
		shell->p_size = 0;
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
