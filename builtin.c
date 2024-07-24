#include "minishell.h"

int	is_builtin(char *cmd)
{
	const char	*cmds[] = {"echo", "cd", "pwd", "export", "unset", "env", "exit"};
	size_t		len;
	int			index;

	index = 0;
	while (index < 7)
	{
		len = ft_strlen(cmds[index]);
		if (!ft_memcmp(cmds[index], cmd, len + 1))
			return (TRUE);
		if (!ft_memcmp(cmds[index], cmd, len) && cmd[len] == ' ')
			return (TRUE);
		index++;
	}
	return (FALSE);
}

int	exec_builtin(t_process p, t_data *d)
{
	const char	*cmds[] = {"echo", "cd", "pwd", "export", "unset", "env", "exit"};
	int			index;

	index = 0;
	// printf("builtin?\n");
	while (index < 7)
	{
		if (!ft_memcmp(cmds[index], p.path, ft_strlen(cmds[index]) + 1))
			break ;
		index++;
	}
	return (find_builtin(index)(p, d));
}
