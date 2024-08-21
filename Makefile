CC = cc
CFLAGS = -Wall -Wextra -Werror -g
CPPFLAGS = -I${HOME}/.brew/opt/readline/include -Iminishell.h -Ift_err.h -I./execute/execute.h -I./src/parsing.h -I./utils/terminal.h -MMD -MP
LDFLAGS = -L${HOME}/.brew/opt/readline/lib -Llibft -L$(DEQDIR)
LDLIBS = -lreadline -lft -lftdeque
BUILTINDIR = ./src/execute/builtin/
EXECDIR = ./src/execute/
PARSEDIR = ./src/parsing/
SRCDIR = ./src/
OBJDIR = ./obj/
UTILSDIR = ./utils/
DEQDIR = ./src/execute/deque
SRCS = ./main.c \
		./execute.c \
		./free.c \
		./setting.c \
		./process.c \
		./process_utils.c \
		./validation.c \
		./terminal.c \
		$(UTILSDIR)print.c \
		$(UTILSDIR)term.c \
		$(UTILSDIR)utils.c \
		$(UTILSDIR)signal.c \
		$(UTILSDIR)signal_utils.c \
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
		$(PARSEDIR)parsing.c \
		$(PARSEDIR)get_buffer.c \
		$(PARSEDIR)pipe.c \
		$(PARSEDIR)redirect.c \
		$(PARSEDIR)argv.c \
		$(PARSEDIR)wordend.c \
		$(PARSEDIR)wordlen.c \
		$(PARSEDIR)tokenizer.c \
		$(PARSEDIR)token_utils.c \
		$(PARSEDIR)token_lst.c \
		$(PARSEDIR)parser.c \
		$(PARSEDIR)replace_value_quote.c \
		$(PARSEDIR)replace_value_utils.c \
		$(PARSEDIR)resword.c \
		$(PARSEDIR)utils.c \
		$(EXECDIR)heredoc.c \
		$(EXECDIR)heredoc_signal.c \
		$(EXECDIR)execute.c \
		$(EXECDIR)cmdline.c \
		$(EXECDIR)cmdline_utils.c \
		$(EXECDIR)redirect.c \
		$(EXECDIR)redirect_utils.c \
		$(EXECDIR)free.c \
		$(EXECDIR)next_cmd.c \

OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)
NAME = minishell

all : $(NAME)

-include $(DEPS)

$(NAME) : $(OBJS)
	make -C libft
	make -C $(DEQDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) $^ -o $@


$(EXECDIR)%.o : $(EXECDIR)%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILTINDIR)%.o : $(BUILTINDIR)%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(UTILSDIR)%.o : $(UTILSDIR)%.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean :
	make clean -C libft
	make clean -C $(DEQDIR)
	rm -f $(OBJS) $(DEPS)

fclean :
	make fclean -C libft
	make fclean -C $(DEQDIR)
	rm -f $(OBJS) $(DEPS) $(NAME)

re : fclean
	make all

.PHONY: all clean fclean re
