#include "../minishell.h"

int	navigate_dir(char *to_dir)
{
	if (access(to_dir, X_OK) || chdir(to_dir) == -1)
		return (handle_error("cd", to_dir, EXTRA_ERROR));
	return (EXIT_SUCCESS);
}

int	set_pwd(t_shell *shell, char *dir)
{
	char	*str;

	if (!read_val_deq(shell->data.envps, "PWD"))
		return (EXIT_SUCCESS);
	str = ft_strjoin("PWD=", dir);
	if (!str || replace_back(shell->data.envps, str))
	{
		free(str);
		return (handle_error("cd", NULL, EXTRA_ERROR));
	}
	free(str);
	return (EXIT_SUCCESS);	
}

int	set_oldpwd(t_shell *shell, char *dir)
{
	char	*str;
	t_deque	*deq;

	deq = find_deq(shell->data.envps, "OLDPWD");
	if (!deq)
		return (EXIT_SUCCESS);
	if (!deq->state)
		return (replace_back(shell->data.envps, "OLDPWD="));
	str = ft_strjoin("OLDPWD=", dir);
	if (!str || replace_back(shell->data.envps, str))
	{
		free(str);
		return (EXIT_FAILURE);
	}
	free(str);
	return (EXIT_SUCCESS);	
}

int	navigate_var(t_deques *envps, char *var)
{
	if (!read_val_deq(envps, var))
		return (handle_error("cd", var, NOT_SET));
	return (navigate_dir(read_val_deq(envps, var)));
}

int	change_cwd(t_shell *shell, char *to_dir)
{
	char	*dest;

	dest = NULL;
	if (!to_dir || !ft_memcmp(to_dir, "--", 3))
		return (navigate_var(shell->data.envps, "HOME"));
	else if (!ft_memcmp(to_dir, "-", 2))
		return (navigate_var(shell->data.envps, "OLDPWD"));
	return (navigate_dir(to_dir));
}

// relative path
int ft_cd(t_shell *shell, t_process p)
{
	char	*cwd;
	int		status;
	int		index;

	status = EXIT_SUCCESS;
	cwd = NULL;
	index = 1;
	if (set_cwd(&cwd))
		return (handle_error("cd", NULL, EXTRA_ERROR));
	if (!ft_memcmp(p.args[index], "--", 3) && p.args[index + 1])
		index++;
	if (change_cwd(shell, p.args[1]))
	{
		free(cwd);
		return (EXTRA_ERROR);
	}
	else if (set_oldpwd(shell, cwd))
	{
		free(cwd);
		return (handle_error("cd", NULL, EXTRA_ERROR));
	}
	free(cwd);
	if (set_cwd(&cwd) || set_pwd(shell, cwd))
	{
		free(cwd);
		return (handle_error("cd", NULL, EXTRA_ERROR));
	}
	free(cwd);
	return (EXIT_SUCCESS);
}