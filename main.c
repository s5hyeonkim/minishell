/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 16:43:13 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/27 11:42:38 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* main */

void	set_shell(t_shell *shell, char *envp[])
{
	ft_memset(shell, 0, sizeof(t_shell));
	set_data(shell, envp);
}

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
	rl_replace_line("", 0);
	add_history(*buffer);
}

void	loop(t_shell *shell)
{
	char	*buffer;
	int		code;

	while (1)
	{
		clean_buffer(shell);
		readlines(shell, &buffer);
		code = parselines(&(shell->t), shell->data.envps, buffer);
		free(buffer);
		if (!code)
			exec_cmds(shell);
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	t_shell	shell;

	(void)argv;
	init(&shell, argc, envp);
	loop(&shell);
}
