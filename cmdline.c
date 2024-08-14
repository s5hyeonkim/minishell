/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdline.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 07:42:29 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/14 07:42:36 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

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

// redirection 추가 필요
int	check_quotation_flag(int *flag, int flag2)
{
	*flag = !(*flag);
	if (!flag2)
		return (TRUE);
	return (FALSE);
}

void	parsing_cmd(char *cmd, size_t *start, size_t *end)
{
	size_t		size;
	int			flag;
	int			flag1;

	size = *start;
	flag = 0;
	flag1 = 0;
	while (cmd[size] == ' ')
		size++;
	*start = size;
	while (cmd[size])
	{
		if (cmd[size] == '\'' && check_quotation_flag(&flag, flag1))
			size++;
		if (cmd[size] == '\"' && check_quotation_flag(&flag1, flag))
			size++;
		if (!flag1 && !flag && cmd[size] == ' ')
			break ;
		size++;
	}
	*end = size;
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
		return (push_keyval(deqs, cmd, 0, ""));
	while (fb[0] < len)
	{
		parsing_cmd(cmd, &fb[0], &fb[1]);
		if (fb[0] >= len)
			break ;
		str = ft_substr(cmd, fb[0], fb[1] - fb[0]);
		if (!str || push_keyval(deqs, str, 0, ""))
		{
			free(str);
			return (EXTRA_ERROR);
		}
		free(str);
		fb[0] = ++fb[1];
	}
	return (EXIT_SUCCESS);
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
	strs = deqtostrs(deqs);
	// printf("%s\n", strs[0]);
	free_deques(&deqs);
	return (strs);
}
