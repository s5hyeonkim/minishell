#include "minishell.h"
static int	get_depth(char *str)
{
	int num;

	num = 0;
	if (!str)
		return (0);
	while (*str && num < 1000)
	{
		if (!ft_isdigit(*str))
			return (0);
		num = num * 10 + *str - '0';
		str++;
	}
	if (num >= 1000)
		return (0);
	return (num);
}

static void	set_depth(t_shell *shell)
{
	int		depth;
	char	*itoa;

	depth = get_depth(read_val_deq(shell->data.envps, "SHLVL")) + 1;
	if (depth == 1000)
		itoa = ft_strdup("");
	else
		itoa = ft_itoa(depth);
	if (!itoa || replace_back(shell->data.envps, "SHLVL", '=', itoa))
	{
		free(itoa);
		exit_process(shell, NULL, EXTRA_ERROR);
	}
	free(itoa);
}

static void	set_pwd_path(t_shell *shell)
{
	char	*cwd;

	cwd = ft_strdup(shell->data.lcwd);
	if (!cwd || replace_back(shell->data.envps, "OLDPWD", 0, "") || \
	replace_back(shell->data.envps, "PWD", '=', cwd))
	{
		free(cwd);
		exit_process(shell, NULL, EXTRA_ERROR);
	}
	free(cwd);
}

int	set_cwd(char **cwd)
{
	*cwd = ft_calloc(PATH_MAX, sizeof(char));
	if (*cwd == NULL || !getcwd(*cwd, sizeof(char) * PATH_MAX))
		return (EXIT_FAILURE);
	printf ("getcwd: %s\n", *cwd);
	return (EXIT_SUCCESS);
}

static void	set_data(t_shell *shell, char *envp[])
{
	shell->data.envps = strstodeq(envp);
	shell->data.paths = get_env_paths(envp);
	if (!shell->data.paths || !shell->data.envps || set_cwd(&shell->data.lcwd))
		exit_process(shell, NULL, EXTRA_ERROR);
	set_depth(shell);
	set_pwd_path(shell);
}

void	set_shell(t_shell *shell, char *envp[])
{
	ft_memset(shell, 0, sizeof(t_shell));
	set_data(shell, envp);
}