/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubin <yubin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 11:35:18 by yubshin           #+#    #+#             */
/*   Updated: 2024/06/03 14:46:52 by yubin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>
# include <unistd.h>

# ifndef INT_MIN
#  define INT_MIN -2147483648
#  define INT_MAX 2147483647
# endif

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

int			ft_isalpha(int c);
int			ft_isdigit(int c);
int			ft_isalnum(int c);
int			ft_isascii(int c);
int			ft_isprint(int c);
int			ft_isspace(char **str);

size_t		ft_strlen(const char *str);
size_t		ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t		ft_strlcat(char *dst, const char *src, size_t dstsize);

int			ft_toupper(int c);
int			ft_tolower(int c);

char		*ft_strchr(const char *str, int c);
char		*ft_strrchr(const char *str, int c);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
char		*ft_strnstr(const char *haystack, const char *needle, size_t len);

void		*ft_memset(void *ptr, int c, size_t len);
void		ft_bzero(void *str, size_t n);
void		*ft_memcpy(void *dst, const void *src, size_t n);
void		*ft_memmove(void *dst, const void *src, size_t len);
void		*ft_memchr(const void *str, int c, size_t n);
int			ft_memcmp(const void *s1, const void *s2, size_t n);

long long	ft_atoi(const char *str);

void		*ft_calloc(size_t count, size_t size);
void	    *ft_realloc(void *ptr, size_t size);
char		*ft_strdup(const char *s1);

char		*ft_substr(char const *s, unsigned int start, size_t len);
char		*ft_strjoin(char const *s1, char const *s2);
char		*ft_strtrim(char const *s1, char const *set);

char		**ft_split(char const *str, char c);
char		**ft_split_set(char *str, char *charset);

char		*ft_itoa(int n);
char		*ft_strmapi(char const *str, char (*f)(unsigned int, char));
void		ft_striteri(char *str, void (*f)(unsigned int, char*));

void		ft_putchar_fd(char c, int fd);
void		ft_putstr_fd(char *s, int fd);
void		ft_putendl_fd(char *s, int fd);
void		ft_putnbr_fd(int n, int fd);

t_list		*ft_lstnew(void *content);
void		ft_lstadd_front(t_list **lst, t_list *newlst);
int			ft_lstsize(t_list *lst);
t_list		*ft_lstlast(t_list *lst);
void		ft_lstadd_back(t_list **lst, t_list *newlst);
void		ft_lstdelone(t_list *lst, void (*del)(void*));
void		ft_lstclear(t_list **lst, void (*del)(void*));
void		ft_lstiter(t_list *lst, void (*f)(void *));
t_list		*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

int			ft_malloc_str(char **str, long long malloc_size);
void		ft_swap(int *a, int *b);

void	free_str(char *str);
void	free_strs(char **strs);

#endif 
