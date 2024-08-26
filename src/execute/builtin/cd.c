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
	while (is_valid_folder(wd, now, next) && next)
	{
		if (check_folder(wd))
			break ;
		now = next + 1;
		next = ft_strchr(now, '/');
	}
	return (wd);
}

int	navigate_dir(char *to_dir, char *origin)
{
	if (check_folder(to_dir) == EXTRA_ERROR || chdir(to_dir) == -1)
		return (handle_error("cd", origin, EXTRA_ERROR));
	return (EXIT_SUCCESS);
}

int	navigate_var(t_deques *envps, char *var, char **nwd)
{
	if (!ft_strlen(read_val_deq(envps, var)))
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

	index = 1;
	deqs = d->envps;
	if (!is_valid_opt(p.exec.args[1]))
		return (handle_error(p.exec.args[0], p.exec.args[1], INVALID_OPT));
	if (p.exec.args[1] && !ft_memcmp(p.exec.args[1], "--", 3) && p.exec.args[2])
		index++;
	nwd = navigate_targetdir(*d, p.exec.args[index]);
	if (!nwd || set_env_pwd(d->envps, &d->lcwd, &nwd))
		return (EXIT_FAILURE);
	if (p.exec.args[index] && !ft_memcmp(p.exec.args[index], "-", 2))
		ft_putendl_fd(d->lcwd, p.fd.in[1]);
	return (EXIT_SUCCESS);
}
