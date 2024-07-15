#include "../minishell.h"

int	navigate_dir(char *to_dir)
{
	if (access(to_dir, X_OK) || chdir(to_dir) == -1)
		return (handle_error("cd", to_dir, EXTRA_ERROR));
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

int	set_pwd(t_deques *deqs, char *key, char *val)
{
	t_map	keyval;

	if (!find_deq(deqs, key))	
		return (EXIT_SUCCESS);
	if (set_keyval(&keyval, key, '=', val) || replace_back(deqs, keyval))
	{
		free_map(&keyval);
		return (EXTRA_ERROR);
	}
	return (EXIT_SUCCESS);
}

// static int	is_folder(char *path, size_t len)
// {
// 	char	*dir;
// 	struct stat	sb;

// 	dir = ft_substr(path, 0, len);
// 	if (!dir)
// 		return (EXTRA_ERROR);
// 	if ((stat(dir, &sb)))
// 	{
// 		if (((sb.st_mode & S_IFMT) != S_IFDIR))
// 			return (FALSE);
// 		return (TRUE);
// 	}
// 	return (EXTRA_ERROR);
// }

void	set_wd(char *wd, char *now, size_t len)
{
	char	*end;

	end = strrchr(wd, '/');
	printf("%zu %s\n", len, now);
	if (len == 2 && !ft_memcmp(now, "..", len))
	{
		if (end != wd && end)
			end[0] = 0;
		else if (end == wd)
			end[1] = 0;
		printf("conv: %s\n", wd);
		return ;
	}
	if (len == 1 && !ft_memcmp(now, ".", len))
		return ;
	ft_strlcat(wd, now, ft_strlen(wd) + len + 1);
	ft_strlcat(wd, "/", PATH_MAX);
}

char	*next_dir(char *path, char *cwd)
{
	char	*now;
	char	*next;
	char	*wd;

	wd = ft_calloc(PATH_MAX, sizeof(char));
	if (!wd)
		return (NULL);
	now = path;
	next = ft_strchr(now, '/');	
	if (next != now)
		ft_memcpy(wd, cwd, sizeof(char) * ft_strlen(cwd));
	ft_strlcat(wd, "/", PATH_MAX);
	printf("cp: %s\n", wd);
	// next == NULL 따로 계산 필요 그냥ㅇ size_t로 할까
	while (TRUE)
	{
		if (now != next)
			set_wd(wd, now, next - now);
		if (!next)
			break ;
		now = next + 1;
		next = ft_strchr(now, '/');
	}
	if (wd[ft_strlen(wd) - 1] == '/')
		wd[ft_strlen(wd) - 1] = 0;
	return (wd);
}

// path cwd로 불러와서 하나씩 없애기
// symbolic path
int ft_cd(t_shell *shell, t_process p)
{
	char	**wd;
	char	*d;
	int		index;

	index = 1;
	if (p.args[1] && !ft_memcmp(p.args[1], "--", 3) && p.args[2])
		index = 2;
	wd = ft_calloc(3, sizeof(char *));
	if (!wd || set_cwd(&wd[0]))
	{
		free_strs(wd);
		return (handle_error("cd", NULL, EXTRA_ERROR));
	}
	d = next_dir(p.args[index], wd[0]);
	printf("cwd 변환: %s\n", d);
	free(d);
	if (change_cwd(shell, p.args[1]))
	{
		free_strs(wd);
		return (EXTRA_ERROR);
	}
	if (set_cwd(&wd[1]) || set_pwd(shell->data.envps, "OLDPWD", wd[0]) \
	|| set_pwd(shell->data.envps, "PWD", wd[1]))
	{
		free_strs(wd);
		return (handle_error("cd", NULL, EXTRA_ERROR));
	}
	free_strs(wd);
	return (EXIT_SUCCESS);
}