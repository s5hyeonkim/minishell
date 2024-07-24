/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 19:31:24 by sohykim           #+#    #+#             */
/*   Updated: 2024/07/23 19:31:27 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "execute.h"

static int	check_folder(char *to_dir)
{
	DIR	*dir;

	dir = opendir(to_dir);
	if (!dir)
		return (EXTRA_ERROR);
	closedir(dir);
	if (access(to_dir, X_OK))
		return (EXTRA_ERROR);
	return (EXIT_SUCCESS);
}

int	navigate_dir(char *to_dir, char *origin)
{
	// printf("to_dir: %s\n", to_dir);
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

int	ft_cd(t_process p, t_data *d)
{
	char		*nwd;
	int			index;
	t_deques	*deqs;

	index = 1;
	deqs = d->envps;
	nwd = NULL;
	if (p.args[1] && !ft_memcmp(p.args[1], "--", 3) && p.args[2])
		index = 2;
	if (navigate_targetdir(*d, p.args[index], &nwd))
	{
		free(nwd);
		return (EXTRA_ERROR);
	}
	if (find_deq(deqs, "OLDPWD") && replace_back(deqs, "OLDPWD", '=', d->lcwd))
	{
		free(d->lcwd);
		d->lcwd = nwd;
		return (handle_error("cd", NULL, EXTRA_ERROR));
	}
	free(d->lcwd);
	d->lcwd = nwd;
	if (find_deq(deqs, "PWD") && replace_back(deqs, "PWD", '=', d->lcwd))
		return (handle_error("cd", NULL, EXTRA_ERROR));
	return (EXIT_SUCCESS);
}
