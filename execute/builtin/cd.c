/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 19:31:24 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/14 19:18:15 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../execute.h"

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
		ft_memcpy(wd, cwd, ft_strlen(cwd));
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

int	navigate_dir(char *to_dir, char *origin)
{
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

char	*navigate_targetdir(t_data d, char *to_dir)
{
	char	*nwd;
	int		status;

	status = EXIT_SUCCESS;
	nwd = NULL;
	if (!to_dir || !ft_memcmp(to_dir, "--", 3))
		status = navigate_var(d.envps, "HOME", &nwd);
	else if (!ft_memcmp(to_dir, "-", 2))
		status = navigate_var(d.envps, "OLDPWD", &nwd);
	else if (!status)
		nwd = get_nextdir(to_dir, d.lcwd);
	if (!status && !nwd)
	{
		handle_error("cd", NULL, EXTRA_ERROR);
		return (NULL);
	}
	if (!status && !navigate_dir(nwd, to_dir))
		return (nwd);
	free(nwd);
	return (NULL);
}

int	ft_cd(t_process p, t_data *d)
{
	char		*nwd;
	int			index;
	t_deques	*deqs;
	int			status;

	index = 1;
	deqs = d->envps;
	if (!is_valid_opt(p.args[1]))
		return (handle_error(p.args[0], p.args[1], INVALID_OPT));
	if (p.args[1] && !ft_memcmp(p.args[1], "--", 3) && p.args[2])
		index++;
	nwd = navigate_targetdir(*d, p.args[index]);
	if (!nwd)
		return (EXIT_FAILURE);
	status = set_env_pwd(deqs, "OLDPWD", d->lcwd);
	free(d->lcwd);
	d->lcwd = nwd;
	if (status || set_env_pwd(deqs, "PWD", d->lcwd))
		return (handle_error("cd", NULL, EXTRA_ERROR));
	return (EXIT_SUCCESS);
}
