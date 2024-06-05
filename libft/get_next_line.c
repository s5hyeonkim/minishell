/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 18:30:06 by yubin             #+#    #+#             */
/*   Updated: 2024/05/26 23:51:34 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	ft_strchr_gnl(char *str, char chr)
{
	int	strlen;
	int	strchr;

	strlen = 0;
	while (str && str[strlen] != '\0')
	{
		if (chr != '\0' && chr == str[strlen])
		{
			strchr = strlen + 1;
			return (strchr);
		}
		strlen++;
	}
	if (chr == '\0')
		return (strlen);
	return (-1);
}

void	ft_strlcat_gnl(char *dst, const char *src, int dstsize)
{
	int	dstlen;
	int	dstidx;
	int	srcidx;

	dstlen = ft_strchr_gnl(dst, '\0');
	dstidx = dstlen;
	srcidx = 0;
	while (src && dstlen + srcidx + 1 < dstsize)
	{
		dst[dstidx] = src[srcidx];
		dstidx++;
		srcidx++;
	}
	dst[dstidx] = '\0';
}

int	ft_malloc_join_rcd(t_list **fdlst_ptr, int rcdlen, int buflen)
{
	char	*join_rcd;

	if (BUFFER_SIZE > 10)
		(*fdlst_ptr)->mallen = (rcdlen + buflen);
	else if (BUFFER_SIZE <= 10 && (rcdlen + buflen) >= (*fdlst_ptr)->mallen)
		(*fdlst_ptr)->mallen = (rcdlen + buflen) * 10;
	else
		return (0);
	join_rcd = (char *)malloc(sizeof(char) * ((*fdlst_ptr)->mallen + 1));
	if (join_rcd == NULL)
		return (-1);
	join_rcd[0] = '\0';
	if ((*fdlst_ptr)->record)
	{
		ft_strlcat_gnl(join_rcd, (*fdlst_ptr)->record, rcdlen + 1);
		free((*fdlst_ptr)->record);
	}
	(*fdlst_ptr)->record = join_rcd;
	return (1);
}

int	ft_record(t_list **fdlst_ptr, char *buf, int rcdlen, int buflen)
{
	if ((*fdlst_ptr)->readrt == -1)
	{
		free((*fdlst_ptr)->record);
		(*fdlst_ptr)->record = NULL;
		return (-1);
	}
	if (buf[0] == '\0')
		return (0);
	if (*fdlst_ptr != NULL && (*fdlst_ptr)->record != NULL)
		rcdlen = ft_strchr_gnl((*fdlst_ptr)->record, '\0');
	buflen = ((*fdlst_ptr)->readrt);
	if (ft_malloc_join_rcd(fdlst_ptr, rcdlen, buflen) == -1)
		return (-1);
	if (*fdlst_ptr && (*fdlst_ptr)->record)
		ft_strlcat_gnl((*fdlst_ptr)->record, buf, (rcdlen + buflen + 1));
	return (1);
}

char	*get_next_line(int fd)
{
	static t_list	*rcdlst;
	t_list			*fdlst;
	char			buf[BUFFER_SIZE + 1];
	char			*res;

	if (ft_fdlst(&rcdlst, &fdlst, fd) == -1)
		return (NULL);
	buf[0] = '\0';
	res = NULL;
	while (fdlst->readrt > 0 && !ft_strchr(buf, '\n'))
	{
		fdlst->readrt = read(fd, buf, BUFFER_SIZE);
		if (rcdlst->record == NULL && fdlst->readrt == 0)
			return (0);
		if (fdlst->readrt > 0)
			buf[fdlst->readrt] = '\0';
		if (ft_record(&fdlst, buf, 0, 0) == -1)
		{
			ft_freelst(&rcdlst, &fdlst, res);
			return (0);
		}
	}
	ft_strdup_gnl(&rcdlst, &fdlst, &res);
	ft_freelst(&rcdlst, &fdlst, res);
	return (res);
}
