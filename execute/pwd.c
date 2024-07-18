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
int	ft_pwd(t_shell *shell, t_process p)
{
	if (!is_valid_option(p.args[1]))
		return (handle_error(p.args[0], p.args[1], INVALID_OPT));
	printf("%s\n", shell->data.lcwd);
	return (EXIT_SUCCESS);
}