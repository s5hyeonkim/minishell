/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubin <yubin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 18:30:55 by yubin             #+#    #+#             */
/*   Updated: 2024/01/05 18:58:38 by yubin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

int	ft_newadd_lst(t_list **rcdlst_ptr, t_list **fdlst_ptr, int fd)
{
	t_list	*lastlst;

	*fdlst_ptr = (t_list *)malloc(sizeof(t_list));
	if (*fdlst_ptr == NULL)
		return (-1);
	(*fdlst_ptr)->fd = fd;
	(*fdlst_ptr)->readrt = 1;
	(*fdlst_ptr)->mallen = 0;
	(*fdlst_ptr)->record = NULL;
	(*fdlst_ptr)->next = NULL;
	if (*rcdlst_ptr == NULL)
	{
		*rcdlst_ptr = *fdlst_ptr;
		*fdlst_ptr = *fdlst_ptr;
	}
	else
	{
		lastlst = *rcdlst_ptr;
		while (lastlst != NULL && lastlst->next != NULL)
			lastlst = lastlst->next;
		lastlst->next = *fdlst_ptr;
	}
	return (0);
}

int	ft_fdlst(t_list **rcdlst_ptr, t_list **fdlst_ptr, int fd)
{
	t_list	*fdlst;

	fdlst = *rcdlst_ptr;
	while (fdlst != NULL)
	{
		if (fdlst->fd == fd)
		{
			*fdlst_ptr = fdlst;
			return (0);
		}
		fdlst = fdlst->next;
	}
	if (ft_newadd_lst(rcdlst_ptr, fdlst_ptr, fd) == -1)
		return (-1);
	return (0);
}

int	ft_parse_rcd(t_list **fdlst_ptr, char **res_ptr, int reslen)
{
	char	*record;
	int		rcdlen;
	int		cpyidx;

	rcdlen = ft_strchr((*fdlst_ptr)->record, '\0') - reslen;
	record = (char *)malloc(sizeof(char) * (*fdlst_ptr)->mallen + 1);
	if (record == NULL)
	{
		free(*res_ptr);
		*res_ptr = NULL;
		return (-1);
	}
	record[0] = '\0';
	cpyidx = 0;
	while (((*fdlst_ptr)->record + reslen)[cpyidx] != '\0' && cpyidx < rcdlen)
	{
		record[cpyidx] = ((*fdlst_ptr)->record + reslen)[cpyidx];
		cpyidx++;
	}
	record[cpyidx] = '\0';
	free((*fdlst_ptr)->record);
	(*fdlst_ptr)->record = record;
	return (0);
}

int	ft_strdup(t_list **rcdlst_ptr, t_list **fdlst_ptr, char **res_ptr)
{
	int	reslen;
	int	residx;

	if (!((*fdlst_ptr && (*fdlst_ptr)->record)) && \
	(ft_strchr((*fdlst_ptr)->record, '\n') || \
	ft_strchr((*fdlst_ptr)->record, '\0')))
		return (0);
	reslen = ft_strchr((*fdlst_ptr)->record, '\n');
	if (reslen == -1)
		reslen = ft_strchr((*fdlst_ptr)->record, '\0');
	*res_ptr = (char *)malloc(sizeof(char) * reslen + 1);
	if (*res_ptr == NULL)
		return (-1);
	residx = 0;
	while (residx < reslen)
	{
		(*res_ptr)[residx] = ((*fdlst_ptr)->record)[residx];
		residx++;
	}
	(*res_ptr)[residx] = '\0';
	if (ft_parse_rcd(fdlst_ptr, res_ptr, reslen) == -1)
		return (-1);
	if ((*fdlst_ptr)->record[0] == '\0')
		ft_freelst(rcdlst_ptr, fdlst_ptr, NULL);
	return (0);
}

void	ft_freelst(t_list **rcdlst_ptr, t_list **fdlst_ptr, char *res)
{
	t_list	*front_fdlst;

	if (res == NULL && *fdlst_ptr && (*fdlst_ptr)->record)
	{
		free((*fdlst_ptr)->record);
		(*fdlst_ptr)->record = NULL;
	}
	if (res == NULL && *fdlst_ptr)
	{
		front_fdlst = (*rcdlst_ptr);
		while (front_fdlst && front_fdlst->next \
		&& front_fdlst->next->fd != (*fdlst_ptr)->fd)
			front_fdlst = front_fdlst->next;
		if ((*rcdlst_ptr) == (*fdlst_ptr))
			(*rcdlst_ptr) = (*rcdlst_ptr)->next;
		else if ((*fdlst_ptr)->next == NULL)
			front_fdlst->next = NULL;
		else
			front_fdlst->next = (*fdlst_ptr)->next;
		free(*fdlst_ptr);
		(*fdlst_ptr) = NULL;
	}
	return ;
}
