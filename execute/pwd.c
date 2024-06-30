#include "../minishell.h"

int	is_valid_option(char *opt)
{
	if (!opt)
		return (TRUE);
	if (!ft_memcmp(opt, "--", 3) || !ft_memcmp(opt, "-", 2) || opt[0] != '-')
		return (TRUE);
	return (FALSE);
}

char	*get_pwd(t_exec *info)
{
	char	cwd[4096];
	char	*pwd;

	ft_memset(cwd, 0, 4096);
	if (!getcwd(cwd, sizeof(cwd)))
		exit_process(info, NULL, EXTRA_ERROR);
	pwd = ft_strdup(cwd);
	if (!pwd)
		exit_process(info, NULL, MALLOC_FAILED);
	return (pwd);
}

int	ft_pwd(t_exec *info, t_process p)
{
	char	*cwd;

	(void) info;
	if (!is_valid_option(p.args[1]))
		return (INVALID_ARGV);
	cwd = get_pwd(info);
	printf("%s\n", cwd);
	free(cwd);
	return (EXIT_SUCCESS);
}