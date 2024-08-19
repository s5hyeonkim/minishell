/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 07:02:29 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/19 18:27:46 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	wait_process(t_shell *shell)
{
	size_t	index;
	int		status;

	index = 0;
	while (index < shell->p_size && shell->p[index].pid)
	{
		waitpid(shell->p[index].pid, &status, 0);
		if (!WIFSIGNALED(status) && WIFEXITED(status))
			g_status = WEXITSTATUS(status);
		index++;
	}
}

void	subprocess(t_shell *shell)
{
	size_t	index;

	index = 0;
	set_signal_sub(handler_sub);
	while (index < shell->p_size)
	{
		if (open_pipe(&shell->p[index], shell->p_size) \
				|| fork_process(&shell->p[index]))
			break ;
		child(shell, index);
		parent(shell, index);
		index++;
	}
	wait_process(shell);
	set_signal_init(handler_init);
}

//free 추가해주기
void	inprocess(t_shell *shell)
{
	long	status;

	status = exec_builtin(shell->p[0], &shell->data);
	if (!ft_memcmp(shell->p[0].args[0], "exit", 5) && !status)
	{
		clean_files(shell->p, shell->p_size);
		reset_terminal(shell);
		exit_wo_error(shell, g_status);
	}
	g_status = status;
}

int	token_to_process(t_shell *shell, t_token *t, size_t *index)
{
	int	status;

	status = EXIT_SUCCESS;
	if (t->type == T_SIMPLE_CMD)
	{
		if (token_to_word(&shell->p[*index], shell->data, t->right))
			return (EXTRA_ERROR);
		status = find_redirect(&shell->p[*index], t->left);
		shell->p[*index].index = *index;
		*index += 1;
		return (status);
	}
	if (t->left)
		status = token_to_process(shell, t->left, index);
	if (!status && t->right)
		status = token_to_process(shell, t->right, index);
	return (status);
}

// void print_token(t_token *t)
// {
// 	const char *d[] = {"", "|", "simple cmd",
// 	"cmd word", "redirects", ">>", "<<", ">", "<"};
// 	if (!t)
// 		return;
// 	printf("type is %s\n", d[t->type]);
// 	if (t->word)
// 	{
// 		printf("token word: %s\n", t->word);
// 		for (int i = 0; t->argvs[i] != 0; i++)
// 			printf("args: %s\n", t->argvs[0]);
// 	}
// 	print_token(t->left);
// 	if (t->right && t->right->type == T_SIMPLE_CMD)
// 		printf("\n\n\n");
// 	print_token(t->right);	
// }

// 명령어 없을때 한번더 체크하기
void	exec_cmds(t_shell *shell)
{
	size_t	index;
	int		status;

	index = 0;
	shell->p_size = find_pipe(shell->t);
	shell->p = ft_calloc(shell->p_size + 1, sizeof(t_process));
	status = 0;
	if (shell->p)
		status = token_to_process(shell, shell->t, &index);
	if (!shell->p || status)
	{
		g_status = EXIT_FAILURE;
		if (status != SIGNALED)
			handle_error(NULL, NULL, EXTRA_ERROR);
	}
	else if (shell->p_size == 1 && is_builtin(shell->p[0].path))
		inprocess(shell);
	else
		subprocess(shell);
}

// char	**debug(char *path, char **args)
// {
	// size_t	index;
	// size_t	size;
	// size_t	add;
	// char	**debug;
// 
	// index = 0;
	// size = 0;
	// add = 0;
	// while (args[size])
		// size++;
	// debug = ft_calloc(size + 2, sizeof(char *));
	// if (ft_memcmp(path, args[0], ft_strlen(path) + 1))
		// debug[add++] = ft_strdup(path);
	// while (index < size)
	// {
		// debug[index + add] = args[index];
		// index++;
	// }
	// debug[++index] = 0;
	// return (debug);
// }
