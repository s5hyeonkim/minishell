/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 16:35:46 by yubshin           #+#    #+#             */
/*   Updated: 2024/08/21 12:46:59 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	set_signal(void (*handler)(int), int signo)
{
	t_sigaction	action;

	action.sa_handler = handler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	if (sigaction(signo, &action, NULL) == (int)SIG_ERR)
		return (EXTRA_ERROR);
	return (EXIT_SUCCESS);
}

int	set_signal_init(void (*handler)(int))
{
	int	status;

	terminal_printoff();
	status = set_signal(handler, SIGINT);
	if (!status)
		status = set_signal(handler, SIGTERM);
	if (!status)
		status = set_signal(SIG_IGN, SIGQUIT);
	return (status);
}

int	set_signal_sub(void (*handler)(int))
{
	int	status;

	terminal_printon();
	status = set_signal(handler, SIGINT);
	if (!status)
		status = set_signal(handler, SIGQUIT);
	return (status);
}

void	handler_init(int signo)
{
	if (signo == SIGINT)
	{
		replace_line(TRUE);
		g_status = EXIT_FAILURE;
	}
	if (signo == SIGTERM)
	{
		move_cursor();
		ft_putstr_fd("exit\n", STDERR_FILENO);
	}
}

void	handler_sub(int signo)
{
	g_status = signo + SIGEXIT;
	if (signo == SIGINT)
		replace_line(FALSE);
	if (signo == SIGQUIT)
		ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
}
