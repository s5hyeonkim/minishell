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
	char	*env_depth;

	depth = get_depth(read_val_deq(shell->data.envps, "SHLVL")) + 1;
	if (depth == 1000)
		itoa = ft_strdup("");
	else
		itoa = ft_itoa(depth);
	if (!itoa)
		exit_process(shell, NULL, EXTRA_ERROR);
	env_depth = ft_strjoin("SHLVL=", itoa);
	free(itoa);
	if (!env_depth || replace_back(shell->data.envps, env_depth))
	{
		free(env_depth);
		exit_process(shell, NULL, EXTRA_ERROR);
	}
	free(env_depth);
}

static void	set_pwd_path(t_shell *shell)
{
	char	*cwd;
	char	*env;

	cwd = NULL;
	if (replace_back(shell->data.envps, "OLDPWD"))
		exit_process(shell, NULL, EXTRA_ERROR);
	if (set_cwd(&cwd))
	{
		free(cwd);
		exit_process(shell, NULL, EXTRA_ERROR);
	}
	env = ft_strjoin("PWD=", cwd);
	free(cwd);
	if (!env || replace_back(shell->data.envps, env))
	{
		free(env);
		exit_process(shell, NULL, EXTRA_ERROR);
	}
	free(env);
}

static void	set_data(t_shell *shell, char *envp[])
{
	shell->data.envps = strstodeq(envp);
	shell->data.paths = get_env_paths(envp);
	if (!shell->data.paths || !shell->data.envps)
		exit_process(shell, NULL, EXTRA_ERROR);
	set_depth(shell);
	set_pwd_path(shell);
}

void	set_shell(t_shell *shell, char *envp[])
{
	ft_memset(shell, 0, sizeof(t_shell));
	set_data(shell, envp);
}