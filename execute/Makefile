CC = cc
CFLAGS = -Wall -Wextra -Werror
CPPFLAGS = -I../libft/libft.h -MMD -MP
LDFLAGS = -L../libft
LDLIBS = -lft
SRCS = echo.c \
	   cd.c \
	   pwd.c \
	   export.c \
	   unset.c \
	   env.c \
	   exit.c

OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)
NAME = $(OBJS:.o=)

all : libft $(NAME)

-include $(DEPS)

libft :
	make -C ../libft

$(NAME): %: %.c
	$(CC) $(CFLAG) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) -o $@ $<

clean :
	make clean -C ../libft
	rm -f $(OBJS)
	rm -f $(DEPS)

fclean : clean
	rm -f ../libft/libft.a
	rm -f $(NAME)

re : fclean
	make all

.PHONY : all clean fclean re