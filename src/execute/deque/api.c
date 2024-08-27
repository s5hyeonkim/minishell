/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   api.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 18:41:44 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/14 18:42:56 by sohykim          ###   ########.fr       */
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

char	**deqtostrs(t_deque *dq)
{
	char	**ret;
	size_t	id;
	size_t	len;

	len = ft_deqlen(dq);
	ret = ft_calloc(len + 1, sizeof(char *));
	if (!ret)
		return (NULL);
	id = 0;
	while (id < len)
	{
		if (((t_map *)dq->data)->state == ENV || ((t_map *)dq->data)->state == EXPORT)
		{
			ret[id] = ft_pairjoin(dq->data);
			if (!ret[id])
			{
				free_strs(ret);
				return (NULL);
			}
		}
		id++;
		dq = dq->next;
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
			free_map(&keyval);
			free_deques(&new);
			return (NULL);
		}
		index++;
	}
	return (new);
}

char	*read_val_deq(t_deques *deqs, char *key)
{
	t_deque	*node;
	t_map	*keyval;

	node = find_deq(deqs, key);
	if (node)
	{
		keyval = (t_map *)node->data;
		return (keyval->val);
	}
	return (NULL);
}

char	*get_val_deq(t_deques *deqs, char *key)
{
	char	*val;
	t_deque	*node;
	t_map	*keyval;

	node = find_deq(deqs, key);
	if (node)
	{
		keyval = (t_map *)node->data;
		val = ft_strdup(keyval->val);
	}
	else
		val = ft_strdup("");
	return (val);
}
