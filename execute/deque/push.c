/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 16:50:52 by sohykim           #+#    #+#             */
/*   Updated: 2024/07/11 12:50:48 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "deque.h"

static int	set_deque(t_deque **deq, t_map keyval)
{

	*deq = create_deque();
	if (*deq == NULL)
		return (EXTRA_ERROR);
	ft_memcpy(&((*deq)->keyval), &keyval, sizeof(t_map));
	set_state(*deq);
	return (EXIT_SUCCESS);
}

int	push_back(t_deques *deques, t_map keyval)
{
	t_deque	*new_deque;

	if (set_deque(&new_deque, keyval))
	{
		free_map(&keyval);
		free_deque(new_deque);
		return (EXTRA_ERROR);
	}
	if (deques->head == NULL)
	{
		deques->head = new_deque;
		new_deque->next = new_deque;
		new_deque->prev = new_deque;
	}	
	else
	{
		new_deque->prev = deques->tail;
		new_deque->next = deques->head;
		deques->head->prev = new_deque;
		deques->tail->next = new_deque;
	}
	deques->tail = new_deque;
	return (EXIT_SUCCESS);
}

int	replace_back(t_deques *deqs, t_map keyval)
{
	if (find_deq(deqs, keyval.key))
		remove_targetdeq(deqs, find_deq(deqs, keyval.key));
	return (push_back(deqs, keyval));
}