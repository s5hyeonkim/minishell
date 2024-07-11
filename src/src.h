#ifndef SRC_H
# define SRC_H

# include <term.h>

//minishell.h로 옮길 것 
typedef	struct termios	t_termios;

void	set_terminal_printoff(void);
void	set_terminal_printon(void);
void	move_cursor(void);

#endif
