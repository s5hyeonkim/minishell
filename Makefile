NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror
CPPFLAGS = ${READLINE_INC} ${MINISHELL_INC} ${DEPFLAGS}
LDFLAGS = ${READLINE_LIB_DIR} ${MINISHELL_LIB_DIR}
LDLIBS = -lreadline -lft -lftdeque
DEPFLAGS = -MMD -MP

READLINE_INC = -I${HOME}/.brew/opt/readline/include
READLINE_LIB_DIR = -L${HOME}/.brew/opt/readline/lib
MINISHELL_INC = -Iminishell.h \
				-Ift_err.h \
				-I./src/execute/execute.h \
				-I./src/parsing/parsing.h \
				-I./utils/terminal.h \
				-I./utils/ft_signal.h 
MINISHELL_LIB_DIR = -Llibft \
					-L${DEQDIR}

BUILTINDIR = ./src/execute/builtin/
DEQDIR = ./src/execute/deque
EXECDIR = ./src/execute/
PARSEDIR = ./src/parsing/
UTILSDIR = ./utils/

SRCS = ./main.c \
		./execute.c \
		./free.c \
		./setting.c \
		./process.c \
		./process_utils.c \
		./set_cmd.c \
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
		$(BUILTINDIR)export_utils.c \
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
		$(PARSEDIR)replace_word.c \
		$(PARSEDIR)replace_quote.c \
		$(PARSEDIR)replace_value.c \
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
