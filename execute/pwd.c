#include "../minishell.h"

int	is_valid_option(char *opt)
{
	if (!opt)
		return (TRUE);
	if (!ft_memcmp(opt, "--", 3) || !ft_memcmp(opt, "-", 2) || opt[0] != '-')
		return (TRUE);
	return (FALSE);
}

//PATH_MAX value need to be changed
int	set_cwd(char **cwd)
{
	*cwd = ft_calloc(PATH_MAX, sizeof(char));
	if (*cwd == NULL || !getcwd(*cwd, sizeof(char) * PATH_MAX))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	ft_pwd(t_shell *shell, t_process p)
{
	char	*cwd;

	(void) shell;
	cwd = NULL;
	if (!is_valid_option(p.args[1]))
		return (handle_error(p.args[0], p.args[1], INVALID_OPT));
	if (set_cwd(&cwd))
	{
		free(cwd);
		return (handle_error(p.args[0], NULL, EXTRA_ERROR));
	}
	printf("%s\n", cwd);
	free(cwd);
	return (EXIT_SUCCESS);
}