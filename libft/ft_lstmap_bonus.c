/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 13:39:41 by yubshin           #+#    #+#             */
/*   Updated: 2023/11/02 12:53:52 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*newlst;
	t_list	*newtemp;
	void	*newcont;

	newlst = 0;
	if (f == NULL || del == NULL)
		return (NULL);
	while (lst)
	{
		newcont = (*f)(lst-> content);
		newtemp = ft_lstnew(newcont);
		if (newtemp == NULL)
		{
			del(newcont);
			ft_lstclear(&newlst, del);
			return (NULL);
		}
		ft_lstadd_back(&newlst, newtemp);
		lst = lst -> next;
	}
	return (newlst);
}
