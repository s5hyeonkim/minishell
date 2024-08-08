/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 19:31:30 by sohykim           #+#    #+#             */
/*   Updated: 2024/07/23 19:31:33 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "execute.h"

int	is_no_nl(char *str)
{
	if (str && !ft_memcmp(str, "-n", 3))
		return (TRUE);
	return (FALSE);
}

char	*read_val_strs(char *strs[], char *key)
{
	char	*ret;
	int		index;
	size_t	size;

	index = 0;
	ret = NULL;
	if (!strs)
		return (NULL);
	while (strs[index])
	{
		size = ft_strchr(strs[index], '=') - strs[index];
		if (!ft_memcmp(strs[index], key, size) && ft_strlen(key) == size)
		{
			ret = strs[index] + size + 1;
			break ;
		}
		index++;
	}
	return (ret);
}

int	ft_echo(t_process p, t_data *d)
{
	int	index;

	(void) d;
	index = 1;
	if (is_no_nl(p.args[1]))
		index++;
	while (p.args[index])
	{
		ft_putstr_fd(p.args[index], p.redirect_fd[1]);
		if (p.args[++index])
			ft_putchar_fd(' ', p.redirect_fd[1]);
	}
	if (!is_no_nl(p.args[1]))
		ft_putchar_fd('\n', p.redirect_fd[1]);
	return (EXIT_SUCCESS);
}
