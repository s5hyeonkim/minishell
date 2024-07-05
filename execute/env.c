#include "../minishell.h"

int	ft_env(t_exec *info, t_process p)
{
	int		index;
	char	**envs;

	if (p.args[1])
		return (INVALID_ARGV);
	envs = deqtoenvp(info->data.envps, ENV);
	if (!envs)
		exit_process(info, NULL, MALLOC_FAILED);
	index = 0;
	while (envs[index])
		printf("%s\n", envs[index++]);
	free(envs);
    return (EXIT_SUCCESS);
}