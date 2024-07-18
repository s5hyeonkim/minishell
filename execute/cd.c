#include "../minishell.h"

static int	check_folder(char *to_dir)
{
	DIR	*dir;

	dir = opendir(to_dir);
	if (!dir)
	{
		printf("no dir\n");
		return (EXTRA_ERROR);
	}
	closedir(dir);
	if (access(to_dir, X_OK))
		return (EXTRA_ERROR);
	return (EXIT_SUCCESS);
}

int	navigate_dir(char *to_dir, char *origin)
{
	printf("to_dir: %s\n", to_dir);
	if (check_folder(to_dir) || chdir(to_dir) == -1)
		return (handle_error("cd", origin, EXTRA_ERROR));
	return (EXIT_SUCCESS);
}

int	navigate_var(t_deques *envps, char *var, char **nwd)
{
	if (!read_val_deq(envps, var))
		return (handle_error("cd", var, NOT_SET));
	*nwd = get_val_deq(envps, var);
	if (*nwd == NULL)
		return (handle_error("cd", NULL, EXTRA_ERROR));
	return (navigate_dir(*nwd, read_val_deq(envps, var)));
}

void	parsing_dir(char *wd, char *now, size_t len)
{
	char	*end;

	end = strrchr(wd, '/');
	if (len == 2 && !ft_memcmp(now, "..", len))
	{
		if (end != wd && end)
			end[0] = 0;
		else if (end == wd)
			end[1] = 0;
		return ;
	}
	if (len == 1 && !ft_memcmp(now, ".", len))
		return ;
	if (wd[ft_strlen(wd) - 1] != '/')
		ft_strlcat(wd, "/", ft_strlen(wd) + 2);
	ft_strlcat(wd, now, ft_strlen(wd) + len + 1);
}

char	*get_nextdir(char *path, char *cwd)
{
	char	*now;
	char	*next;
	char	*wd;

	wd = ft_calloc(PATH_MAX, sizeof(char));
	if (!wd)
		return (NULL);
	now = path;
	next = ft_strchr(now, '/');
	if (next != path)
		ft_memcpy(wd, cwd, sizeof(char) * ft_strlen(cwd));
	else
		ft_strlcat(wd, "/", PATH_MAX);
	while (next)
	{
		if (now != next)
			parsing_dir(wd, now, next - now);
		if (check_folder(wd))
			return (wd);
		now = next + 1;
		next = ft_strchr(now, '/');
	}
	parsing_dir(wd, now, ft_strlen(now));
	check_folder(wd);
	return (wd);
}

int	navigate_targetdir(t_data d, char *to_dir, char **nwd)
{
	int		status;

	if (!to_dir || !ft_memcmp(to_dir, "--", 3))
		return (navigate_var(d.envps, "HOME", nwd));
	else if (!ft_memcmp(to_dir, "-", 2))
		return (navigate_var(d.envps, "OLDPWD", nwd));
	*nwd = get_nextdir(to_dir, d.lcwd);
	if (*nwd == NULL)
		return (handle_error("cd", NULL, EXTRA_ERROR));
	printf("next dir: %s\n", *nwd);
	status = navigate_dir(*nwd, to_dir);
	return (status);	
}


int ft_cd(t_shell *shell, t_process p)
{
	char	*nwd;
	int		index;
	t_data	d;

	index = 1;
	d = shell->data;
	nwd = NULL;
	if (p.args[1] && !ft_memcmp(p.args[1], "--", 3) && p.args[2])
		index = 2;
	if (navigate_targetdir(shell->data, p.args[index], &nwd))
	{
		free(nwd);
		return (EXTRA_ERROR);
	}
	if (find_deq(d.envps, "OLDPWD") && replace_back(d.envps, "OLDPWD", '=', d.lcwd))
	{
		free(shell->data.lcwd);
		shell->data.lcwd = nwd;
		return (handle_error("cd", NULL, EXTRA_ERROR));
	}
	free(shell->data.lcwd);
	shell->data.lcwd = nwd;
	if (find_deq(d.envps, "PWD") && replace_back(d.envps, "PWD", '=', shell->data.lcwd))
		return (handle_error("cd", NULL, EXTRA_ERROR));
	return (EXIT_SUCCESS);
}