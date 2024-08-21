NAME = libft.a

CC = cc
CFLAGS = -Wall -Wextra -Werror -g

AR = ar
AFLAGS = rcs

SRCS_MB = ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c ft_isspace.c \
		ft_strlen.c ft_strlcpy.c ft_strlcat.c \
		ft_toupper.c ft_tolower.c \
		ft_strchr.c ft_strrchr.c ft_strncmp.c ft_strnstr.c \
		ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c ft_memchr.c ft_memcmp.c \
		ft_atoi.c ft_calloc.c ft_realloc.c\
		ft_strdup.c ft_substr.c ft_strjoin.c ft_strtrim.c ft_strsdup.c \
		ft_split.c ft_itoa.c \
		ft_strmapi.c ft_striteri.c \
		ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
		ft_lstnew_bonus.c ft_lstadd_front_bonus.c ft_lstsize_bonus.c \
		ft_lstlast_bonus.c ft_lstadd_back_bonus.c ft_lstdelone_bonus.c \
		ft_lstclear_bonus.c ft_lstiter_bonus.c ft_lstmap_bonus.c \
		get_next_line.c get_next_line_utils.c \
		ft_malloc_str.c \
		ft_swap.c \

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
