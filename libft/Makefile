NAME = libft.a

CC = cc
CFLAGS = -Wall -Wextra -Werror -g

AR = ar
AFLAGS = rcs

SRCS_MB = ft_isalpha.c ft_isdigit.c ft_isspace.c \
		ft_strlen.c ft_strlcpy.c ft_strlcat.c \
		ft_strchr.c ft_strrchr.c \
		ft_memset.c ft_bzero.c ft_memcpy.c ft_memcmp.c \
		ft_calloc.c \
		ft_strdup.c ft_substr.c ft_strjoin.c ft_strtrim.c \
		ft_split.c ft_itoa.c \
		ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c \

OBJS_MB = ${SRCS_MB:.c=.o}

all: ${NAME}

${NAME}: mandatory

%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@
	
mandatory: ${OBJS_MB}
	$(AR) ${AFLAGS} $(NAME) $^
	@touch $@

clean: 
	rm -f ${OBJS_MB} mandatory bonus

fclean: 
	make clean
	rm -f ${NAME} 

re: 
	make fclean
	make all

.PHONY: all clean fclean re 
