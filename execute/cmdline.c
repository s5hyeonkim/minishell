/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdline.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 07:42:29 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/19 15:52:26 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "execute.h"

char	*get_pathcmd(char **paths, char *cmd)
{
	int		index;
	char	*ret;

	index = 0;
	while (paths[index])
	{
		ret = ft_pairjoin(paths[index++], '/', cmd);
		if (!ret || !access(ret, X_OK))
			return (ret);
		free(ret);
	}
	return (ft_strdup(cmd));
}

char	*get_cmdpath(char **paths, char *cmd)
{
	char	*ret;

	if (is_builtin(cmd))
		ret = ft_strdup(cmd);
	else
		ret = get_pathcmd(paths, cmd);
	return (ret);
}

// argv 파싱할때 가공하는거에서 quotation 없으면 가공안하는 걸로 바꿔주기

char	**get_cmdargs(char **cmds)
{
	t_deques	*deqs;
	size_t		index;
	char		**strs;

	deqs = create_deques();
	index = 0;
	if (!deqs)
		return (NULL);
	while (cmds[index])
	{
		if (set_parsing_deques(deqs, cmds[index]))
		{
			free_deques(&deqs);
			return (NULL);
		}
		index++;
	}
	strs = deqtostrs(deqs->head);
	free_deques(&deqs);
	return (strs);
}
