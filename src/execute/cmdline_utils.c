/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdline_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 15:52:32 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/27 11:27:24 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

int	is_valid_quotation(size_t *start, int *open1, int open2)
{
	*open1 = !(*open1);
	if (!open2)
	{
		if (!(*open1))
			(*start)++;
		return (TRUE);
	}
	return (FALSE);
}

int	check_quotation_flag(int *flag, int flag2)
{
	*flag = !(*flag);
	if (!flag2)
		return (TRUE);
	return (FALSE);
}

void	parsing_cmd(char *cmd, size_t *start, size_t *end)
{
	int			flag;
	int			flag1;

	flag = 0;
	flag1 = 0;
	while (cmd[*start] == ' ')
		(*start)++;
	*end = *start;
	while (cmd[*end])
	{
		if (cmd[*end] == '\'' && check_quotation_flag(&flag, flag1))
			(*end)++;
		else if (cmd[*end] == '\"' && check_quotation_flag(&flag1, flag))
			(*end)++;
		else if (!flag1 && !flag && cmd[*end] == ' ')
			break ;
		else
			(*end)++;
	}
}

int	set_parsing_deques(t_deques *deqs, char *cmd)
{
	size_t		fb[2];
	size_t		len;
	char		*str;

	len = ft_strlen(cmd);
	ft_memset(fb, 0, sizeof(size_t) * 2);
	str = NULL;
	if (!ft_strchr(cmd, '\'') && !ft_strchr(cmd, '\"'))
		return (push_keyval(deqs, cmd, "", NO));
	while (fb[0] < len)
	{
		parsing_cmd(cmd, &fb[0], &fb[1]);
		if (fb[0] >= len)
			break ;
		str = ft_substr(cmd, fb[0], fb[1] - fb[0]);
		if (!str || push_keyval(deqs, str, "", NO))
		{
			free(str);
			return (EXTRA_ERROR);
		}
		free(str);
		fb[0] = ++fb[1];
	}
	return (EXIT_SUCCESS);
}

char	*ft_triplejoin(char *s1, char *s2, char *s3)
{
	size_t	len;
	char	*ret;

	len = ft_strlen(s1);
	len += ft_strlen(s2);
	len += ft_strlen(s3);
	ret = ft_calloc(len + 1, sizeof(char));
	if (!ret)
		return (NULL);
	ft_strlcat(ret, s1, len + 1);
	ft_strlcat(ret, s2, len + 1);
	ft_strlcat(ret, s3, len + 1);
	return (ret);
}
