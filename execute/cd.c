#include "../minishell.h"

int	navigate_dir(char *to_dir)
{
	if (!to_dir)
		return (handle_error("cd", NULL, EXTRA_ERROR));
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

	if (!read_val_deq(shell->data.envps, "OLDPWD"))
		return (EXIT_SUCCESS);
	str = ft_strjoin("OLDPWD=", dir);
	if (!str || replace_back(shell->data.envps, str))
	{
		free(str);
		return (handle_error("cd", NULL, EXTRA_ERROR));
	}
	free(str);
	return (EXIT_SUCCESS);	
}

int	navigate_var(t_shell *shell, char *var)
{
	if (!read_val_deq(shell->data.envps, var))
		return (handle_error("cd", var, NOT_SET));
	return (navigate_dir(read_val_deq(shell->data.envps, var)));
}

int	to_dest(t_shell *shell, char *to_dir)
{
	char	*dest;

	dest = NULL;
	if (!to_dir || !ft_memcmp(to_dir, "--", 3))
		return (navigate_var(shell, "HOME"));
	else if (!ft_memcmp(to_dir, "-", 2))
		return (navigate_var(shell, "OLDPWD"));
	// else if (!ft_memcmp(to_dir, "~/", 2) || !ft_memcmp(to_dir, "~", 2))
		// return (navigate_dir(ft_strjoin(shell->data.home, &to_dir[1])));
	return (navigate_dir(to_dir));
}

// relative path
int ft_cd(t_shell *shell, t_process p)
{
	char	*cwd;
	int		status;

	status = EXIT_SUCCESS;
	if (to_dest(shell, p.args[1]))
		return (BUILTIN_ERROR);
	else if (!set_cwd(&cwd))
	{
		status = set_oldpwd(shell, cwd);
		free(cwd);
		if (!set_cwd(&cwd))
		{
			status = set_pwd(shell, cwd);
			free(cwd);
			return (status);
		}
	}
	handle_error("cd", NULL, EXTRA_ERROR);
	return (BUILTIN_ERROR);
}