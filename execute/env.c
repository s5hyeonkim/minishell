#include "../libft/libft.h"
#include <stdio.h>

int	main(int argc, char *argv[], char *envp[])
{
	int	index;

    if (argc != 1)
	{
		ft_putstr_fd("env: ", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (1);
	}
	index = 0;
	while (envp[index])
		printf("%s\n", envp[index++]);
    return (0);
}