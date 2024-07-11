#include "../minishell.h"

int ft_unset(t_shell *shell, t_process p)
{
	int		index;
	int		status;
	char	*key;

	index = 0;
	status = EXIT_SUCCESS;
	if (!p.args[1] || !ft_memcmp(p.args[1], "--", 3))
		return (status);
	while (p.args[++index])
	{
		if (!ft_isalpha(p.args[index][0]))
			status = handle_error(p.args[0], p.args[index], INVALID_IDF);
		else
		{
			key = get_key(p.args[index]);
            if (!key)
                return (handle_error(p.args[0], NULL, EXTRA_ERROR));
			remove_targetdeq(shell->data.envps, find_deq(shell->data.envps, key));
		}
	}
	return (status);
}