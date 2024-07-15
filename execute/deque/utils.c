/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 16:50:52 by sohykim           #+#    #+#             */
/*   Updated: 2024/07/11 12:50:53 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "deque.h"

size_t	ft_deqlen(t_deque *head)
{
	t_deque	*deq;
	size_t	size;

	if (!head)
		return (0);
	size = 1;
	deq = head;
	while (deq != head->prev)
	{
		size++;
		deq = deq->next;
	}
	return (size);
}

char	**deqtostrs(t_deques *deqs)
{
	char	**ret;
	size_t	index;
	size_t	size;
	t_deque	*deq;
	t_map	keyval;

	size = ft_deqlen(deqs->head);
	ret = ft_calloc(size + 1, sizeof(char *));
	if (!ret)
		return (NULL);
	index = 0;
	deq = deqs->tail;
	while (index < size)
	{
		deq = deq->next;
		keyval = deq->keyval;
		ret[index] = ft_pairjoin(keyval.key, keyval.mid, keyval.val);
		if (!ret[index++])
		{
			free_strs(ret);
			return (NULL);
		}
	}
	if (!size)
	{
		ret[size] = ft_strdup("");
		if (!ret[size])
		{
			free_strs(ret);
			ret = NULL;
		}
	}
	return (ret);
}

t_deques	*strstodeq(char **strs)
{
	t_deques	*new;
	t_map		keyval;
	int			index;

	index = 0;
	new = create_deques();
	if (!new)
		return (NULL);
	while (strs[index])
	{
		if (set_map(&keyval, strs[index]) || push_back(new, keyval))
		{
			free_deques(&new);
			return (NULL);
		}
		index++;
	}
	if (!index && (set_map(&keyval, "") || push_back(new, keyval)))
	{
		free_deques(&new);
		return (NULL);
	}
	return (new);
}

char	*read_val_deq(t_deques *deqs, char *key)
{
	char	*val;
	t_deque	*node;

	val = NULL;
	node = find_deq(deqs, key);
	if (node)
		val = node->keyval.val;
	return (val);
}

void	set_state(t_deque *deq)
{
	deq->state = EXPORT;
	if (deq->keyval.mid == '=')
		deq->state = ENV;
}