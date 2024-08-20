/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 16:50:52 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/14 18:43:20 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "deque.h"

void	set_state(t_deque *deq)
{
	deq->state = EXPORT;
	if (deq->keyval.mid == '=')
		deq->state = ENV;
}

int	is_valid_key(char *s)
{
	if (ft_isdigit(*s) || *s == '=')
		return (FALSE);
	while (*s)
	{
		if (*s == '=')
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

	node = deq->tail;
	while (node)
	{
		node = node->next;
		if (!ft_memcmp(node->keyval.key, key, ft_strlen(key) + 1))
			return (node);
		if (node == deq->tail)
			break ;
	}
	return (NULL);
}
