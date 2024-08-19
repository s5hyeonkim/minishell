#ifndef FT_SIGNAL_H
# define FT_SIGNAL_H
# include <signal.h>
# include <sys/signal.h>

/* signal.c */
int			set_signal(void(*handler)(int), int signo);
int			set_signal_init(void(*handler)(int));
int			set_signal_sub(void(*handler)(int));
void		handler_init(int signo);
void		handler_sub(int signo);

#endif