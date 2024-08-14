CC = cc
CFLAGS = -Wall -Wextra -Werror -g
CPPFLAGS = -I${HOME}/.brew/opt/readline/include -Iminishell.h -Ift_err.h -I./execute/execute.h -I./src/parsing.h -MMD -MP
LDFLAGS = -L${HOME}/.brew/opt/readline/lib -Llibft -Lexecute/deque
LDLIBS = -lreadline -lft -lftdeque
BUILTINDIR = ./execute/builtin/
EXECDIR = ./execute/
SRCDIR = ./src/
OBJDIR = ./obj/
SRCS = ./main.c \
		./execute.c \
		./free.c \
		./setting.c \
		./process.c \
		./pipeline.c \
		./print.c \
		./redirect.c \
		./heredoc.c \
		$(BUILTINDIR)utils.c \
		$(BUILTINDIR)cd.c \
		$(BUILTINDIR)cd_utils.c \
		$(BUILTINDIR)echo.c \
		$(BUILTINDIR)env.c \
		$(BUILTINDIR)execute.c \
		$(BUILTINDIR)exit.c \
		$(BUILTINDIR)export.c \
		$(BUILTINDIR)pwd.c \
		$(BUILTINDIR)unset.c \
		$(BUILTINDIR)builtin.c \
		$(SRCDIR)signal.c \
		$(SRCDIR)signal_utils.c \
		$(SRCDIR)terminal.c \
		$(SRCDIR)parsing.c \
		$(SRCDIR)quote.c \
		$(SRCDIR)syntax.c \
		$(SRCDIR)tlst.c \
		$(SRCDIR)debug.c \
		$(SRCDIR)sublibft.c \
		$(SRCDIR)token.c \
		$(SRCDIR)find.c \
		$(SRCDIR)resword.c \
		$(EXECDIR)heredoc_signal.c \
		$(EXECDIR)cmdline.c \
		$(EXECDIR)cmdline_utils.c \

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

$(BUILTINDIR)%.o : $(BUILTINDIR)%.c
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
