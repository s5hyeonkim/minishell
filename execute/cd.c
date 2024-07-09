#include "../minishell.h"

int	move_dir(char *to_dir)
{
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
		free(dir);
		exit_process(info, NULL, MALLOC_FAILED);
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
		free(dir);
		free(str);
		exit_process(info, NULL, MALLOC_FAILED);
	}
	free(str);
	return (EXIT_SUCCESS);	
}


int	to_dest(t_exec *info, char *to_dir)
{
	char	*dest;

	dest = NULL ;
	if (!to_dir || !ft_memcmp(to_dir, "--", 3))
	{
		if (!read_val_deq(info->data.envps, "HOME"))
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
			return (EXIT_FAILURE);
		}
		return (move_dir(dest));
	}
	else if (!ft_memcmp(to_dir, "-", 2))
	{
		if (!read_val_deq(info->data.envps, "OLDPWD"))
		{
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", STDERR_FILENO);
			return (EXIT_FAILURE);
		}
		return (move_dir(read_val_deq(info->data.envps, "OLDPWD")));
	}
	else if (!ft_memcmp(to_dir, "~/", 2) || !ft_memcmp(to_dir, "~", 2))
	{
		dest = ft_strjoin(info->data.home, &to_dir[1]);
		if (!dest)
			return (handle_error("cd", NULL, MALLOC_FAILED));
		if (!move_dir(dest))
		{
			free(dest);
			return (EXIT_SUCCESS);
		}
	}
	else if (!move_dir(to_dir))
		return (EXIT_SUCCESS);
	free(dest);
	return (EXIT_FAILURE);
}

// relative path
int ft_cd(t_exec *info, t_process p)
{
	char	*cwd;

	if (to_dest(info, p.args[1]))
		return (EXIT_FAILURE);
	else if (!set_pwd(&cwd))
	{
		printf("pwd set needed\n");
		set_oldpwd(info, cwd);
		free(cwd);
		if (!set_pwd(&cwd))
		{
			set_newpwd(info, cwd);
			free(cwd);
			return (EXIT_SUCCESS);
		}
	}
	return (handle_error("cd", "pwd", MALLOC_FAILED));
}