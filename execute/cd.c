#include "../minishell.h"

int	navigate_dir(char *to_dir)
{
	if (!to_dir)
		return (handle_error("cd", NULL, EXTRA_ERROR));
	if (access(to_dir, X_OK) || chdir(to_dir) == -1)
		return (handle_error("cd", to_dir, EXTRA_ERROR));
	return (EXIT_SUCCESS);
}

int	set_newpwd(t_exec *info, char *dir)
{
	char	*str;

	if (!read_val_deq(info->data.envps, "PWD"))
		return (EXIT_SUCCESS);
	str = ft_strjoin("PWD=", dir);
	if (!str || push_keyval(info->data.envps, str))
	{
		free(str);
		return (handle_error("cd", NULL, EXTRA_ERROR));
	}
	free(str);
	return (EXIT_SUCCESS);	
}

int	set_oldpwd(t_exec *info, char *dir)
{
	char	*str;

	if (!read_val_deq(info->data.envps, "OLDPWD"))
		return (EXIT_SUCCESS);
	str = ft_strjoin("OLDPWD=", dir);
	if (!str || push_keyval(info->data.envps, str))
	{
		free(str);
		return (handle_error("cd", NULL, EXTRA_ERROR));
	}
	free(str);
	return (EXIT_SUCCESS);	
}

int	navigate_var(t_exec *info, char *var)
{
	if (!read_val_deq(info->data.envps, var))
		return (handle_error("cd", var, NOT_SET));
	return (navigate_dir(read_val_deq(info->data.envps, var)));
}

int	to_dest(t_exec *info, char *to_dir)
{
	char	*dest;

	dest = NULL;
	if (!to_dir || !ft_memcmp(to_dir, "--", 3))
		return (navigate_var(info, "HOME"));
	else if (!ft_memcmp(to_dir, "-", 2))
		return (navigate_var(info, "OLDPWD"));
	else if (!ft_memcmp(to_dir, "~/", 2) || !ft_memcmp(to_dir, "~", 2))
		return (navigate_dir(ft_strjoin(info->data.home, &to_dir[1])));
	return (navigate_dir(to_dir));
}

// relative path
int ft_cd(t_exec *info, t_process p)
{
	char	*cwd;
	int		status;

	status = EXIT_SUCCESS;
	if (to_dest(info, p.args[1]))
		return (BUILTIN_ERROR);
	else if (!set_pwd(&cwd))
	{
		status = set_oldpwd(info, cwd);
		free(cwd);
		if (!set_pwd(&cwd))
		{
			status = set_newpwd(info, cwd);
			free(cwd);
			return (status);
		}
	}
	handle_error("cd", NULL, EXTRA_ERROR);
	return (BUILTIN_ERROR);
}