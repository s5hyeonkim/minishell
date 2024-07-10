#include "../minishell.h"

int	ft_env(t_exec *info, t_process p)
{
	int		index;
	char	**envs;

	if (p.args[1] && (ft_memcmp(p.args[1], "--", 3) || p.args[2]))
	{
		handle_error(p.args[0], NULL, INVALID_OPT);
		return (BUILTIN_ERROR);
	}
	envs = deqtoenvp(info->data.envps, ENV);
	if (!envs)
	{
		handle_error(p.args[0], NULL, EXTRA_ERROR);
		return (BUILTIN_ERROR);
	}
	index = 0;
	while (envs[index])
		ft_putendl_fd(envs[index++], STDOUT_FILENO);
	free(envs);
    return (EXIT_SUCCESS);
}