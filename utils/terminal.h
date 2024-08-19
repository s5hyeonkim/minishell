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