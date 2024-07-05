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

t_deque	*pop_front(t_deques *deques)
{
	t_deque	*node;

	node = NULL;
	if (deques->head)
	{
		node = deques->head;
		if (deques->head == deques->tail)
		{
			deques->tail = NULL;
			deques->head = NULL;
		}
		else
		{
			deques->head = deques->head->next;
			deques->head->prev = deques->tail;
			deques->tail->next = deques->head;
		}
		node->next = NULL;
		node->prev = NULL;
	}
	return (node);
}

t_deque	*pop_back(t_deques *deques)
{
	t_deque	*node;

	node = NULL;
	if (deques->head)
	{
		node = deques->tail;
		if (deques->head == deques->tail)
		{
			deques->tail = NULL;
			deques->head = NULL;
		}
		else
		{
			deques->tail = deques->tail->prev;
			deques->tail->next = deques->head;
			deques->head->prev = deques->tail;
		}
		node->next = NULL;
		node->prev = NULL;
	}
	return (node);
}

t_deque	*search_deq(t_deques *deq, char *key)
{
	t_deque	*node;

	node = deq->tail;
	while (node)
	{
		node = node->next;
		if (ft_strlen(key) == ft_strlen(node->keyval.key) && !ft_memcmp(node->keyval.key, key, ft_strlen(key)))
			return (node);
		if (node == deq->tail)
			break ;
	}
	return (NULL);
}

t_deque	*pop(t_deques *deqs, t_deque *target)
{
	t_deque	*node;

	node = deqs->tail;
	if (deqs->head == target)
		return (pop_front(deqs));
	if (deqs->tail == target)
		return (pop_back(deqs));
	while (node)
	{
		node = node->next;
		if (node == target)
		{
			node->prev->next = node->next;
			node->next->prev = node->prev;
			node->prev = NULL;
			node->next = NULL;
			return (node);
		}
		if (node == deqs->tail)
			break ;
	}
	return (NULL);
}