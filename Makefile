CC = cc
CFLAGS = -Wall -Wextra -Werror 
CPPFLAGS = -I${HOME}/.brew/opt/readline/include -Iminishell.h -MMD -MP
LDFLAGS = -L${HOME}/.brew/opt/readline/lib -Llibft
LDLIBS = -lreadline -lft
SRCS = ./main.c \

OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)

NAME = minishell

all : $(NAME)

-include $(DEPS)

$(NAME) : $(OBJS)
	make -C libft
	make -C execute
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) $^ -o $@


%.o : %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

clean :
	make clean -C libft
	make clean -C execute
	rm -f $(DEPS)
	rm -f $(OBJS)

fclean :
	make fclean -C libft
	make fclean -C execute
	rm -f $(OBJS)
	rm -f $(DEPS)
	rm -f $(NAME)

re : fclean
	make all

.PHONY: all clean fclean re
