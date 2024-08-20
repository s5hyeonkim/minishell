/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wordlen.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 20:18:29 by yubin             #+#    #+#             */
/*   Updated: 2024/08/20 11:07:27 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	wordlen(char *str) //word기준 word의 길이를 구함
{
	int		len;

	len = find_wordend(str) - str;
	if (len == 0)
		len++;
	// printf("len:%d\n", len);
	return (len);
}

 //startstr: pipestart, len: pipestart~end length - 파이프 단위로 시작문자열 && 길이 
char	*wordlen_pipe(char *startstr, int *len)
{
	char 	*pipebuffer_head;
	char	*nowstr;
	int		next;

	*len = 0;
	while (*startstr && \
		(ft_isspace(*startstr) == TRUE || ft_ispipe(*startstr) == TRUE))
		startstr++;
	pipebuffer_head = startstr;
	nowstr = pipebuffer_head;
	while (nowstr && *nowstr)
 	{	
		next = wordlen(nowstr);
		*len += next;
		nowstr += next;
		if (*nowstr == '\0' || ft_ispipe(*nowstr) == TRUE)
			break ;
	}
	return (pipebuffer_head);
}

char	*wordlen_redirect(char *startstr, int *len)
{
	char	*redirect_head;
	char	*nowstr;
	int		next;

	*len = 0;
	redirect_head = find_notspace(startstr);
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

	filename_head = find_filename_head(redirect_head);
	nowstr = filename_head;
	while(*nowstr)
	{
		next = wordlen(nowstr);
		*len += next;
		nowstr += next;
		if (ft_isspace(*nowstr) == TRUE || ft_isredirect(*nowstr) == TRUE)
			break;
	}
	return (filename_head);
}

char	*wordlen_word(char *startstr, int *len)
{
	// char	*filename_head;
	char	*nowstr;
	int		next;

	startstr = find_notspace(startstr);
	nowstr = startstr;
	while(*nowstr)
	{
		next = wordlen(nowstr);
		*len += next;
		nowstr += next;
		if (ft_isspace(*nowstr) == TRUE)
			break;
	}
	return (startstr);
}
