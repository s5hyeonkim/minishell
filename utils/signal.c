/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 16:35:46 by yubshin           #+#    #+#             */
/*   Updated: 2024/08/27 10:16:38 by yubshin          ###   ########.fr       */
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
