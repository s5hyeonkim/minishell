#ifndef UTILS_H
# define UTILS_H
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include "../libft/libft.h"

int	    get_depth(char *str);
char	**get_env_paths(char *envp[]);
char	*read_val_strs(char *strs[], char *key);
void	ft_sort(char **strs, int (*guide)(char *, char *));

#endif