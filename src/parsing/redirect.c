/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 19:43:55 by yubin             #+#    #+#             */
/*   Updated: 2024/08/26 13:12:01 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	*find_redirect_head(char *buffer)
{
	int	len;

	buffer = find_notspace(buffer);
	if (ft_isredirect(*buffer))
		return (buffer);
	buffer = wordlen_redirect(buffer, &len);
	buffer += len;
	return (buffer);
}

char	*find_filename_head(char *redirect_head)
{
	char	*filename_head;
	int		typeno;

	filename_head = redirect_head;
	typeno = read_redirect_typeno(filename_head);
	if (typeno == T_DLESS || typeno == T_DGREAT)
		filename_head += 2;
	else if (typeno == T_LESS || typeno == T_GREAT)
		filename_head += 1;
	filename_head = find_notspace(filename_head);
	return (filename_head);
}

int	read_redirect_typeno(char *str)
{
	if (*str == LESS && *(str + 1) == LESS)
		return (T_DLESS);
	else if (*str == GREAT && *(str + 1) == GREAT)
		return (T_DGREAT);
	else if (*str == LESS)
		return (T_LESS);
	else if (*str == GREAT)
		return (T_GREAT);
	return (EXIT_SUCCESS);
}

int	handle_empty_redirect(char *buffer)
{
	char	*redirect_head;
	char	*filename_head;
	int		typeno;
	int		len;

	while (*buffer)
	{
		redirect_head = find_redirect_head(buffer);
		if (redirect_head == NULL)
			return (EXIT_SUCCESS);
		len = 0;
		filename_head = wordlen_filename(redirect_head, &len);
		if (*redirect_head != '\0' && *filename_head == '\0')
			return (handle_error_withstr(NULL, "newline", 7, SYN_TOK));
		if (ft_isresword(*filename_head) == TRUE)
		{
			len = 1;
			typeno = read_redirect_typeno(redirect_head);
			if (typeno == T_DLESS || typeno == T_DGREAT)
				len = 2;
			return (handle_error_withstr(NULL, filename_head, len, SYN_TOK));
		}
		buffer = filename_head + len;
	}
	return (EXIT_SUCCESS);
}
