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
	size_t	size;

	id = 0;
	size = ft_deqlen(dq);
	ret = ft_calloc(size + 1, sizeof(char *));
	if (!ret)
		return (NULL);
	while (id < size)
	{
		ret[id] = ft_pairjoin(dq->keyval.key, dq->keyval.mid, dq->keyval.val);
		if (!ret[id++])
		{
			free_strs(ret);
			return (NULL);
		}
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
	if (!index && (set_map(&keyval, "") || push_back(new, keyval)))
	{
		free_map(&keyval);
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

char	*get_val_deq(t_deques *deqs, char *key)
{
	char	*val;
	t_deque	*node;

	node = find_deq(deqs, key);
	if (node)
		val = ft_strdup(node->keyval.val);
	else
		val = ft_strdup("");
	return (val);
}
