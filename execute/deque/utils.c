/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 16:50:52 by sohykim           #+#    #+#             */
/*   Updated: 2024/05/23 18:55:54 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "deque.h"

int	get_size(t_deque *head, t_status state)
{
	t_deque	*deq;
	int		size;

	deq = head;
	size = 0;
	while (deq)
	{
		if (deq->state >= state)
			size++;
		if (deq->next == head)
			break ;
		deq = deq->next;
	}
	return (size);
}

char	**deqtoenvp(t_deques *deqs, t_status state)
{
	char	**ret;
	int		size;
	int		index;
	t_deque	*deq;

	size = get_size(deqs->head, state);
	ret = ft_calloc(size + 1, sizeof(char *));
	if (!ret)
		return (NULL);
	index = 0;
	deq = deqs->tail;
	while (index < size)
	{
		deq = deq->next;
		if (deq->state != state)
			continue ;
		ret[index] = ft_pairjoin(deq->keyval);
		if (!ret[index++])
		{
			free_strs(ret);
			return (NULL);
		}
	}
	return (ret);
}

t_deques	*strstodeq(char **strs)
{
	t_deques	*new;
	t_pairs		keyval;
	int			index;

	index = 0;
	new = create_deques();
	if (!new)
		return (NULL);
	while (strs[index])
	{
		if (set_keyval(strs[index], &keyval) \
		|| push_back(new, keyval))
		{
			free_deques(&new);
			return (NULL);
		}
		new->tail->state = ENV;
		index++;
	}
	if (!index && (set_keyval("", &keyval) || push_back(new, keyval)))
	{
		free_deques(&new);
		return (NULL);
	}
	return (new);
}

