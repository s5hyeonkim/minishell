/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 16:50:52 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/26 17:16:33 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "deque.h"

int	is_valid_key(char *s)
{
	if (!ft_isalpha(*s) && *s != '_')
		return (FALSE);
	while (*s)
	{
		if (*s == '=')
			break ;
		if (!ft_memcmp(s, "+=", 2))
			break ;
		if (!ft_isdigit(*s) && !ft_isalpha(*s) && *s != '_')
			return (FALSE);
		s++;
	}
	return (TRUE);
}

t_deque	*find_deq(t_deques *deq, char *key)
{
	t_deque	*node;
	t_map	*keyval;

	node = deq->tail;
	while (node)
	{
		node = node->next;
		keyval = node->data;
		if (!ft_memcmp(keyval->key, key, ft_strlen(key) + 1))
			return (node);
		if (node == deq->tail)
			break ;
	}
	return (NULL);
}
