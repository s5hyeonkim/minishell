/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 20:56:22 by yubshin           #+#    #+#             */
/*   Updated: 2023/11/15 15:15:14 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *newlst)
{
	t_list	*temp;

	if (*lst == NULL)
		*lst = newlst;
	else if (newlst != NULL)
	{
		temp = ft_lstlast(*lst);
		temp -> next = newlst;
	}
}
