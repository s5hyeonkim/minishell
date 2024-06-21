#include "../libft/libft.h"
#include <stdio.h>

void	check_others(char *name, char *opt)
{
	ft_putstr_fd(name, 2);
	if (opt[0] == '-')
	{
		ft_putstr_fd(": illegal option -- ", 2);
		ft_putchar_fd(opt[1], 2);
		ft_putchar_fd('\n', 2);
	}
	else
		ft_putstr_fd("usage: pwd with no option\n", 2);
	exit(1);
}

void 	check_pwd(char *opt)
{
	char	err[3];

	if (opt[0] != '-' || !opt[1] || (opt[1] == '-' && !opt[2]))
		return ;
	err[0] = 0;
	ft_strlcat(err, opt, 3);
	ft_putstr_fd("minishell: pwd: ", 2);
	ft_putstr_fd(err, 2);
	ft_putstr_fd(": invalid option\n", 2);
	ft_putstr_fd("pwd: usage: pwd with no option\n", 2);
	exit(1);
}

void	check_option(char *name, char *opt)
{
	if (!opt)
		return ;
	if (ft_strncmp("pwd", name, 4))
		check_others(name, opt);
	else
		check_pwd(opt);
}

int	main(int argc, char *argv[])
{
	char	cwd[4096];

	check_option(argv[0], argv[1]);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
	{
		perror("minishell: ");
		return (1);
	}
	return (EXIT_SUCCESS);
}