/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 16:15:03 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/14 16:15:05 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

int	check_folder(char *to_dir)
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

int	set_env_pwd(t_deques *deqs, char *key, char *val)
{
	if (find_deq(deqs, key) && replace_back(deqs, key, '=', val))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
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
