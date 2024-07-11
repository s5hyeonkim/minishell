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

size_t	ft_deqlen(t_deque *head, t_state state)
{
	t_deque	*deq;
	size_t	size;

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

char	**deqtoenvp(t_deques *deqs, t_state state)
{
	char	**ret;
	size_t	index;
	size_t	size;
	t_deque	*deq;
	t_map	keyval;

	size = ft_deqlen(deqs->head, state);
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
		keyval = deq->keyval;
		ret[index] = ft_pairjoin(keyval.key, keyval.mid, keyval.val);
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
	t_map		keyval;
	int			index;

	index = 0;
	new = create_deques();
	if (!new)
		return (NULL);
	while (strs[index])
	{
		if (set_map(strs[index], &keyval) \
		|| push_back(new, keyval))
		{
			free_deques(&new);
			return (NULL);
		}
		new->tail->state = ENV;
		index++;
	}
	if (!index && (set_map("", &keyval) || push_back(new, keyval)))
	{
		free_deques(&new);
		return (NULL);
	}
	return (new);
}

void	print_deques(t_deques *deq)
{
	t_deque	*node;

	node  = deq->head;
	while (node)
	{
		if (node->keyval.mid)
			printf("%s=%s\n", node->keyval.key, node->keyval.val);
		else
			printf("%s\n", node->keyval.key);
		node = node->next;
		if (node == deq->head)
			break ;
	}
}

char	*read_val_deq(t_deques *deq, char *key)
{
	char	*val;
	t_deque	*node;

	val = NULL;
	node = find_deq(deq, key);
	if (node)
		val = node->keyval.val;
	return (val);
}
