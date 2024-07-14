CC = cc
CFLAGS = -Wall -Wextra -Werror 
CPPFLAGS = -I${HOME}/.brew/opt/readline/include -Iminishell.h -Ift_err.h -MMD -MP
LDFLAGS = -L${HOME}/.brew/opt/readline/lib -Llibft -Lexecute/deque
LDLIBS = -lreadline -lft -lftdeque
EXECDIR = ./execute/
SRCDIR = ./src/
OBJDIR = ./obj/
SRCS = ./main.c \
		./print.c \
		./execute.c \
		./free.c \
		./setting.c \
		$(EXECDIR)cd.c \
		$(EXECDIR)echo.c \
		$(EXECDIR)env.c \
		$(EXECDIR)execute.c \
		$(EXECDIR)exit.c \
		$(EXECDIR)export.c \
		$(EXECDIR)pwd.c \
		$(EXECDIR)unset.c \
		$(SRCDIR)signal.c \

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


$(EXECDIR)%.o : $(EXECDIR)%.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean :
	make clean -C libft
	make clean -C execute/deque
	rm -f $(OBJS) $(DEPS)

fclean :
	make fclean -C libft
	make fclean -C execute/deque
	rm -f $(OBJS) $(DEPS) $(NAME)

re : fclean
	make all

.PHONY: all clean fclean re
