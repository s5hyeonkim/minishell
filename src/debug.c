#include "../minishell.h"

void debug_buffers(char **buffers)
{
    printf("==== debug_buffers ====\n");
	int i = 0;
	while (buffers[i])
		printf("%s#\n", buffers[i++]);
}

void debug_token(t_token *token)
{
	int i = 0;
    printf("==== debug_token ====\n");
	while (token)
	{
		printf("type: %d#\n", token->type);
		printf("word: %s#\n", token->word);
		while (token->argvs && token->argvs[i])
		{
			printf("argv[%d]: %s#\n", i, token->argvs[i]);
			i++;
		}
		token = token->right;
	}
    printf("==== debug_token complete====\n");
}


void debug_tree(t_token *dsttoken, int leftright, int i)
{
	if(!dsttoken)
		return ;
	if (leftright == 0)
		printf("---- left %d----\n", i);
	else if (leftright == 1)
		printf("---- right %d----\n", i);
	else if (leftright == 2)
		printf("---- node %d----\n", i);

	printf("type: %d#\n", dsttoken->type);
	printf("word: %s#\n", dsttoken->word);

	int j = 0;
	while (dsttoken->argvs && dsttoken->argvs[j])
		printf("argv: %s#\n", dsttoken->argvs[j++]);


	printf("now: %p#\n", dsttoken);
	printf("left: %p#\n", dsttoken->left);
	printf("right: %p#\n\n", dsttoken->right);
	debug_tree(dsttoken->left, 0, i + 1);
	debug_tree(dsttoken->right, 1, i + 1);
}

void	handler_heredoc1(int signo)
{
	if (signo == SIGINT)
	{
		rl_replace_line("", 0);
		exit(1);
	}
}

void	handler_heredoc_wait1(int signo)
{
	if (signo == SIGINT)
		replace_line(FALSE);
}

int	read_next_cmd(char **s)
{
	int		status;
	char	*buffer;

	status = set_signal_init(handler_heredoc1);
	rl_replace_line("", 0);
	buffer = readline("> ");
	while (!buffer)
		buffer = readline("> ");
	*s = buffer;
	return (status);
}

// //
// int	wait_heredoc(t_process p)
// {
// 	int	status;

// 	set_signal_init(handler_heredoc_wait);
// 	waitpid(p.pid, &status, 0);
// 	set_signal_init(handler_sub);
// 	if (WIFEXITED(status))
// 		return (WEXITSTATUS(status));
// 	return (EXIT_FAILURE);
// }


int	set_next_cmd(char **s)
{
	t_process	p;

	if (fork_process(&p))
		return (EXIT_FAILURE);
	if (!p.pid)
		exit(read_next_cmd(s));
	else
		return (wait_heredoc(p));	
}