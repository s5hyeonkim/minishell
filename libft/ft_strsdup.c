#include "libft.h"

char	**ft_strsdup(char **envp)
{
	size_t	len;
	char	**new;

	len = 0;
	while (envp[len])
		len++;
	new = ft_calloc(len + 10, sizeof(char *));
	len = 0;
	if (new)
	{
		while (envp[len])
		{
			new[len] = ft_strdup(envp[len]);
			len++;
		}

	}
	return (new);
}
