CC = cc
CFLAGS = -Wall -Wextra -Werror 
CPPFLAGS = -I${HOME}/.brew/opt/readline/include -Iminishell.h -Ift_err.h -MMD -MP
LDFLAGS = -L${HOME}/.brew/opt/readline/lib -Llibft -Lexecute/deque
LDLIBS = -lreadline -lft -lftdeque
EXEC = ./execute/
SRCS = ./main.c \
	   ./print.c \
	   ./execute.c \
	   ./free.c \
	   $(EXEC)cd.c \
	   $(EXEC)echo.c \
	   $(EXEC)env.c \
	   $(EXEC)execute.c \
	   $(EXEC)exit.c \
	   $(EXEC)export.c \
	   $(EXEC)pwd.c \
	   $(EXEC)unset.c \

#./main.c
	   
OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)
NAME = minishell

all : $(NAME)

-include $(DEPS)

$(NAME) : $(OBJS)
	make -C libft
	make -C execute/deque
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) $^ -o $@


$(EXEC)%.o : $(EXEC)%.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean :
	make clean -C libft
	make fclean -C execute/deque
	rm -f $(DEPS)
	rm -f $(OBJS)

fclean :
	make fclean -C libft
	make fclean -C execute/deque
	rm -f $(OBJS)
	rm -f $(DEPS)
	rm -f $(NAME)

re : fclean
	make all

.PHONY: all clean fclean re
