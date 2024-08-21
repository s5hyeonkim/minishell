/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 10:23:35 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/21 10:23:45 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "terminal.h"

void	terminal_printoff(void)
{
	t_termios	term;

	tcgetattr(STDOUT_FILENO, &term);
	term.c_lflag &= ~(ECHOCTL);
	tcsetattr(STDOUT_FILENO, TCSANOW, &term);
}

void	terminal_printon(void)
{
	t_termios	term;

	tcgetattr(STDOUT_FILENO, &term);
	term.c_lflag |= (ECHOCTL);
	tcsetattr(1, 0, &term);
}
