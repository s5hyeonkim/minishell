#include "minishell.h"

void	clean_cmds(t_shell *shell)
{
	size_t	index;

	index = 0;
	while (index < shell->p_size)
	{
		if (shell->p[index].redirect_fd[0] > 0)
		{
			close(shell->p[index].redirect_fd[0]);
			shell->p[index].redirect_fd[0] = 0;
		}
		if (shell->p[index].redirect_fd[1] > 0)
		{
			close(shell->p[index].redirect_fd[1]);
			shell->p[index].redirect_fd[1] = 0;
		}
		if (shell->p[index].pipe_fd[0] > 0)
		{
			close(shell->p[index].pipe_fd[0]);
			shell->p[index].pipe_fd[0] = 0;
		}
		if (shell->p[index].pipe_fd[1] > 0)
		{
			close(shell->p[index].pipe_fd[1]);
			shell->p[index].pipe_fd[1] = 0;
		}
		index++;
	}
	free_cmds(&shell->t, &shell->p, &shell->p_size);
}

/* parsing and set tokens 수정 필요 */
void	tokenization(t_shell *shell, t_token *t)
{
	printf("origin cmd: %s\n", t->word);
	t->type = T_SIMPLE_CMD;
	(void) shell;
	//tokenization(shell, t->left);
	//tokenization(shell, t->right);
}

// int	set_token(t_token **t)
// {
// 	*t = ft_calloc(1, sizeof(t_token));
// 	if (*t == NULL)
// 		return (EXTRA_ERROR);
// 	return (EXIT_SUCCESS);
// }

/* token lst */
int set_token(t_token **t)
{
	*t = ft_calloc(1, sizeof(t_token));
	if (*t == NULL)
		return (EXTRA_ERROR);
	return (EXIT_SUCCESS);
}

t_token *token_lstlast(t_token *t)
{
	if (t == NULL)
		return (NULL);
	while (t->right != NULL)
		t = t->right;
	return (t);
}

void token_lstadd_back(t_token **t, t_token *newtoken)
{
	t_token *lasttoken;

	if (*t == NULL)
		*t = newtoken;
	else if (newtoken != NULL)
	{
		lasttoken= token_lstlast(*t);
		lasttoken->right = newtoken; 
	}
}

void add_token(t_shell *shell, int type, char *str)
{
	t_token *newtoken;

	set_token(&newtoken);
	newtoken->type = type;
	newtoken->word = str;
	token_lstadd_back(&shell->t, newtoken);
}


// int	token_type(char b1, char b2)
// {
// 	if (b1 == '|' && b2 != '|') // || : or 연산자  |||이상 : syntax error unexpected token '|'
// 		return (T_PIPE);
// 	else if (b1 == '<' && b2 == '<')

// 	else if (b1 == '>' && b2 ==)
// }

//PIPE WORD 

void	set_tokens(t_shell *shell, char *buffer)
{
	if (set_token(&(shell->t)))
		exit_process(shell, NULL, EXTRA_ERROR);
	shell->t->word = buffer;
	tokenization(shell, shell->t);
}

// void lexical_analysis(t_shell *shell, char *buffer)
// {
// 	// if (set_token(&(shell->t)))
// 	// 	exit_process(shell, NULL, EXTRA_ERROR);
// 	// shell->t->word = buffer;
// 	//토큰 리스트에 추가

// }


// void	parselines(t_shell *shell, char *buffer)
// {


// 	// printf("token start %s\n", shell->t->cmd);
	
// 	// printf("token end\n");

// 	lexical_analysis(shell, buffer);
// }


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
		// parselines(shell, buffer);
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
	atexit(leaks);
	t_shell	shell;

	(void)argv;
	init(&shell, argc, envp);
	loop(&shell);
}
