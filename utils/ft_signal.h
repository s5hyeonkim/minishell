/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signal.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 13:41:54 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/21 13:42:32 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SIGNAL_H
# define FT_SIGNAL_H
# include <signal.h>
# include <sys/signal.h>

/* signal.c */
int		set_signal(void (*handler)(int), int signo);
int		set_signal_init(void (*handler)(int));
int		set_signal_sub(void (*handler)(int));
void	handler_init(int signo);
void	handler_sub(int signo);
void	replace_line(int redisplayon);
void	move_cursor(void);

#endif
