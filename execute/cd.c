#include "../minishell.h"

int	move_dir(char *to_dir)
{
	int		stdout_fd;

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

int	set_home(t_exec *info, char **home)
{
	*home = read_val_deq(info->data.envps, "HOME");
	if (*home == NULL)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

//no malloc
char	*read_val_deq(t_deques *deq, char *key)
{
	char	*val;
	t_deque	*node;

	val = NULL;
	node = search_deq(deq, key);
	if (node)
		val = node->keyval.val;
	return (val);
}

int	set_pwd(t_exec *info, char *dir)
{
	char	*str;

	if (!read_val_deq(info->data.envps, "PWD"))
		return (EXIT_SUCCESS);
	str = ft_strjoin("PWD=", dir);
	if (!str || change_envs(info->data.envps, str))
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
	if (!str || change_envs(info->data.envps, str))
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

	dest = NULL;
	if (!to_dir)
	{
		if (!set_home(info, &dest) && !move_dir(dest))
			return (EXIT_SUCCESS);
		return (EXIT_FAILURE);
	}
	else if (!ft_memcmp(to_dir, "~/", 2) || !ft_memcmp(to_dir, "~", 2))
	{
		dest = ft_strjoin(info->data.home, &to_dir[1]);
		if (!dest)
			exit_process(info, NULL, MALLOC_FAILED);
		if (!move_dir(dest))
		{
			free(dest);
			return (EXIT_SUCCESS);
		}
	}
	if (!move_dir(to_dir))
		return (EXIT_SUCCESS);
	free(dest);
	return (EXIT_FAILURE);
}

// relative path
int ft_cd(t_exec *info, t_process p)
{
	int		index;
	char	*cwd;

	index = 1;
	cwd = get_pwd(info);
	printf("cd start\n");
	if (to_dest(info, p.args[1]))
		return (EXIT_FAILURE);
	else
	{
		printf("pwd set needed\n");
		set_oldpwd(info, cwd);
		free(cwd);
		cwd = get_pwd(info);
		set_pwd(info, cwd);
		free(cwd);
	}
	return (EXIT_SUCCESS);
}