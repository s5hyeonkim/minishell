#include "../minishell.h"

int	is_valid_option(char *opt)
{
	if (!opt)
		return (TRUE);
	if (!ft_memcmp(opt, "--", 3) || !ft_memcmp(opt, "-", 2) || opt[0] != '-')
		return (TRUE);
	handle_error("pwd", NULL, INVALID_ARGV);
	return (FALSE);
}

int	set_pwd(char **pwd)
{
	char	cwd[4096];

	ft_memset(cwd, 0, 4096);
	if (!getcwd(cwd, sizeof(cwd)))
		return (handle_error("pwd", NULL, EXTRA_ERROR));
	*pwd = ft_strdup(cwd);
	if (*pwd == NULL)
		return (handle_error("pwd", NULL, MALLOC_FAILED));
	return (EXIT_SUCCESS);
}

int	ft_pwd(t_exec *info, t_process p)
{
	char	*cwd;

	(void) info;
	if (!is_valid_option(p.args[1]) || set_pwd(&cwd))
		return (EXIT_FAILURE);
	printf("%s\n", cwd);
	free(cwd);
	return (EXIT_SUCCESS);
}