#include "../minishell.h"

int	move_dir(char *to_dir)
{
	int	stdout_fd;

	if (access(to_dir, X_OK) || chdir(to_dir) == -1)
	{
		stdout_fd = dup(STDOUT_FILENO);
		dup2(STDERR_FILENO, STDOUT_FILENO);
		printf("minishell: cd: %s: %s\n", to_dir, strerror(errno));
		dup2(stdout_fd, STDOUT_FILENO);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

char	*get_val_fromdeq(t_exec *info, char *key)
{
	char	**envp;

	envp = deqtoenvp(info->data.envps);
	if (!envp)
		exit_process(info, NULL, MALLOC_FAILED);
	return (get_value(envp, key));
}

int	set_pwd(t_exec *info, char *dir)
{
	char	*str;

	if (!get_val_fromdeq(info, "PWD"))
		return (EXIT_SUCCESS);
	str = ft_strjoin("PWD=", dir);
	if (!str || change_envs(info->data.envps, str))
	{
		free(str);
		free(dir);
		exit_process(info, NULL, MALLOC_FAILED);
	}
	return (EXIT_SUCCESS);	
}

int	set_oldpwd(t_exec *info, char *dir)
{
	char	*str;

	if (!get_val_fromdeq(info, "OLDPWD"))
		return (EXIT_SUCCESS);
	str = ft_strjoin("OLDPWD=", dir);
	if (!str || change_envs(info->data.envps, str))
	{
		free(dir);
		free(str);
		exit_process(info, NULL, MALLOC_FAILED);
	}
	return (EXIT_SUCCESS);	
}


// relative path
int ft_cd(t_exec *info, t_process p)
{
	int		index;
	char	*cwd;
	char	*to_dir;

	index = 1;
	cwd = get_pwd(info);
	if (!p.args[1])
	{
		to_dir = get_val_fromdeq(info, "HOME");
		if (!to_dir || move_dir(to_dir))
		{
			if (!to_dir)
				ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
			free(cwd);
			return (EXIT_FAILURE);
		}
	}
	else if (move_dir(p.args[1]))
		return (EXIT_FAILURE);
	else
	{
		set_oldpwd(info, cwd);
		free(cwd);
		cwd = get_pwd(info);
		set_pwd(info, cwd);
		free(cwd);
	}
	return (EXIT_SUCCESS);
}