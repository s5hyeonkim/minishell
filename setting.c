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
	t_map	keyval;

	depth = get_depth(read_val_deq(shell->data.envps, "SHLVL")) + 1;
	if (depth == 1000)
		itoa = ft_strdup("");
	else
		itoa = ft_itoa(depth);
	if (!itoa)
		exit_process(shell, NULL, EXTRA_ERROR);
	if (set_keyval(&keyval, "SHLVL", '=', itoa) || replace_back(shell->data.envps, keyval))
	{
		free(itoa);
		free_map(&keyval);
		exit_process(shell, NULL, EXTRA_ERROR);
	}
	free(itoa);
}

static void	set_pwd_path(t_shell *shell)
{
	t_map	keyval;
	char	*cwd;

	cwd = NULL;
	if (set_keyval(&keyval, "OLDPWD", '=', "") || replace_back(shell->data.envps, keyval) || set_cwd(&cwd))
	{
		free(cwd);
		free_map(&keyval);
		exit_process(shell, NULL, EXTRA_ERROR);
	}
	if (set_keyval(&keyval, "PWD", '=', cwd) || replace_back(shell->data.envps, keyval))
	{
		free(cwd);
		free_map(&keyval);
		exit_process(shell, NULL, EXTRA_ERROR);
	}
	free(cwd);
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