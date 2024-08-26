/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 16:15:03 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/26 17:27:16 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execute.h"

int	check_folder(char *to_dir)
{
	if (!is_folder(to_dir) || access(to_dir, X_OK))
		return (EXTRA_ERROR);
	return (EXIT_SUCCESS);
}

int	set_env_pwd(t_deques *deqs, char **lcwd, char **lnwd)
{
	int	status;

	status = EXIT_SUCCESS;
	if (find_deq(deqs, "OLDPWD") && replace_back(deqs, "OLDPWD", *lcwd, ENV))
		status = handle_error("cd", NULL, EXTRA_ERROR);
	free(*lcwd);
	*lcwd = *lnwd;
	if (!status && find_deq(deqs, "PWD") && \
			replace_back(deqs, "PWD", *lcwd, ENV))
		status = handle_error("cd", NULL, EXTRA_ERROR);
	return (status);
}

void	parsing_dir(char *wd, char *target, size_t len)
{
	char	*end;

	end = strrchr(wd, '/');
	if (len == 2 && !ft_memcmp(target, "..", len))
	{
		if (end != wd && end)
			end[0] = 0;
		else if (end == wd)
			end[1] = 0;
		return ;
	}
	if (len == 1 && !ft_memcmp(target, ".", len))
		return ;
	if (wd[ft_strlen(wd) - 1] != '/')
		ft_strlcat(wd, "/", ft_strlen(wd) + 2);
	ft_strlcat(wd, target, ft_strlen(wd) + len + 1);
}

int	set_cwd(char **cwd)
{
	*cwd = ft_calloc(PATH_MAX, sizeof(char));
	if (*cwd == NULL || !getcwd(*cwd, sizeof(char) * PATH_MAX))
		return (EXTRA_ERROR);
	return (EXIT_SUCCESS);
}

int	is_valid_folder(char *wd, char *now, char *next)
{
	if (next && now != next)
		parsing_dir(wd, now, next - now);
	else if (!next)
		parsing_dir(wd, now, ft_strlen(now));
	return (TRUE);
}
