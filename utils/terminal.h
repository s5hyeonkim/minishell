/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 10:23:55 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/21 10:24:01 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TERMINAL_H
# define TERMINAL_H
# include <sys/ioctl.h>
# include <term.h>
# include <termios.h>
# include <unistd.h>

typedef struct termios		t_termios;
/* terminal.c */
void		terminal_printoff(void);
void		terminal_printon(void);
#endif
