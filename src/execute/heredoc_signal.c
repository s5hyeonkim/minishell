/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_signal.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 15:52:44 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/19 15:52:47 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

void	handler_heredoc(int signo)
{
	if (signo == SIGINT)
	{
		g_status = SIGNALED;
		rl_replace_line("", 0);
		exit(1);
	}
}

void	handler_heredoc_wait(int signo)
{
	if (signo == SIGINT)
	{
		replace_line(FALSE);
		g_status = 1;
	}
}
