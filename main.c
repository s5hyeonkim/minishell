/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 16:43:13 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/20 17:11:29 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* exit */
void	exit_process(t_shell *shell, char *obj, int errcode)
{
	terminal_printon();
	if (errcode && errcode < CMD_NOT_FOUND)
	{
		handle_error(obj, NULL, errcode);
		errcode = EXIT_FAILURE;
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
	g_status = 0;
	if (set_signal_init(handler_init))
		exit_process(shell, NULL, EXTRA_ERROR);
}

void	readlines(t_shell *shell, char **buffer)
{
	*buffer = readline(PROMPT_MSG);
	if (*buffer == 0)
	{
		handler_init(SIGTERM);
		exit_wo_error(shell, g_status);
	}
	if (**buffer)
		add_history(*buffer);
}

void	loop(t_shell *shell)
{
	char	*buffer;
	int		code;
	// rl_clear_history();
	while(1)
	{
		shell->t = 0;
		readlines(shell, &buffer);
		code = parselines(shell, buffer);
		if (code != EXIT_SUCCESS)
		{
			if (code == EXTRA_ERROR)
				handle_error(NULL, NULL, EXTRA_ERROR); //undefined error < 처리 어케 할까 
			free_token(shell->t);
			continue;
		}
		// printf("==loop==\n");
		// debug_tree(shell->t, 2, 0);
		// printf("==loop complete==\n");
		exec_cmds(shell);
		free_token(shell->t);
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
