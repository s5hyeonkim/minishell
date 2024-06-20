CC = cc
CFLAGS = -Wall -Wextra -Werror 
CPPFLAGS = -I${HOME}/.brew/opt/readline/include -Iminishell.h -MMD -MP
LDFLAGS = -L${HOME}/.brew/opt/readline/lib -Llibft
LDLIBS = -lreadline -lft
EXEC_DIR = ./exec/
SRCS = ./main.c \

OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)
NAME = minishell

all : $(NAME)

$(NAME) : $(OBJS)
	make -C libft
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) $^ -o $@

-include $(DEPS)

$(DEQS)%.o : $(DEQS)%.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o : %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

clean :
	make clean -C libft
	rm -f $(DEPS)
	rm -f $(OBJS)

fclean : clean
	rm -f ./libft/libft.a
	rm -f $(NAME)

re : fclean
	make all

.PHONY: all clean fclean re
