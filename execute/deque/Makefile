CC = cc
CFLAGS = -Wall -Wextra -Werror
AR = ar
ARFLAGS = rsc
SRC_DIR = ../../libft
CPPFLAGS = -Ideque.h
LDFLAGS = -L../../libft
LDLIBS = -lft
SRCS = utils.c \
	   creation.c \
	   free.c \
	   pop.c \
	   push.c \
	   utils.c \
	   map.c

OBJS = $(SRCS:.c=.o)
NAME = libftdeque.a

all : $(NAME)

$(NAME) : $(OBJS)
	make -C ../../libft
	$(AR) $(ARFLAGS) $@ $^

%.o : %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $?

clean:
	make clean -C ../../libft
	rm -f $(OBJS)

fclean: clean
	rm -f ../../libft/libft.a
	rm -f $(NAME)

re: fclean
	make all

.PHONY: all clean fclean re
