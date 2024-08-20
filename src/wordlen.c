/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wordlen.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 20:18:29 by yubin             #+#    #+#             */
/*   Updated: 2024/08/20 16:37:41 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	wordlen(char *str)
{
	int		len;

	len = find_wordend(str) - str;
	if (len == 0)
		len++;
	return (len);
}

char	*wordlen_pipe(char *str_head, int *len)
{
	char	*pipe_head;
	char	*nowstr;
	int		next;

	*len = 0;
	pipe_head = str_head;
	while (*pipe_head && \
		(ft_isspace(*pipe_head) == TRUE || ft_ispipe(*pipe_head) == TRUE))
		pipe_head++;
	nowstr = pipe_head;
	while (*nowstr)
	{
		next = wordlen(nowstr);
		*len += next;
		nowstr += next;
		if (ft_ispipe(*nowstr) == TRUE)
			break ;
	}
	return (pipe_head);
}

char	*wordlen_redirect(char *str_head, int *len)
{
	char	*redirect_head;
	char	*nowstr;
	int		next;

	*len = 0;
	redirect_head = find_notspace(str_head);
	nowstr = redirect_head;
	while (*nowstr)
	{
		next = wordlen(nowstr);
		*len += next;
		nowstr += next;
		if (ft_isredirect(*nowstr) == TRUE)
			break ;
	}
	return (redirect_head);
}

char	*wordlen_filename(char *redirect_head, int *len)
{
	char	*filename_head;
	char	*nowstr;
	int		next;

	*len = 0;
	filename_head = find_filename_head(redirect_head);
	nowstr = filename_head;
	while (*nowstr)
	{
		next = wordlen(nowstr);
		*len += next;
		nowstr += next;
		if (ft_isspace(*nowstr) == TRUE || ft_isredirect(*nowstr) == TRUE)
			break ;
	}
	return (filename_head);
}

char	*wordlen_word(char *str_head, int *len)
{
	char	*word_head;
	char	*nowstr;
	int		next;

	*len = 0;
	word_head = find_notspace(str_head);
	nowstr = word_head;
	while (*nowstr)
	{
		next = wordlen(nowstr);
		*len += next;
		nowstr += next;
		if (ft_isspace(*nowstr) == TRUE)
			break ;
	}
	return (word_head);
}
