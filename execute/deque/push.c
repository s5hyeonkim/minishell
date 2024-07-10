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
int	set_deque(t_deque **deq, t_pairs keyval)
{

	*deq = create_deque();
	if (*deq == NULL)
		return (EXTRA_ERROR);
	(*deq)->keyval.key = keyval.key;
	(*deq)->keyval.mid = keyval.mid;
	(*deq)->keyval.val = keyval.val;
	return (EXIT_SUCCESS);
}

int	push_back(t_deques *deques, t_pairs keyval)
{
	t_deque	*new_deque;

	if (set_deque(&new_deque, keyval))
	{
		free_keyval(keyval);
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

int	push_front(t_deques *deques)
{
	t_deque	*new_deque;

	new_deque = create_deque();
	if (new_deque == NULL)
		return (EXTRA_ERROR);
	if (deques->head == NULL)
	{
		deques->tail = new_deque;
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
	deques->head = new_deque;
	return (EXIT_SUCCESS);
}

int	push_keyval(t_deques *deqs, char *str)
{
	t_deque	*node;
	t_pairs	keyval;
	char	*key;

	key = get_key_str(str);
	node = pop(deqs, find_deq(deqs, key));
	free_deque(node);
	free(key);
	if (set_keyval(str, &keyval) || push_back(deqs, keyval))
	{
		free_keyval(keyval);
		return (EXIT_FAILURE);
	}
	if (keyval.val[0])
		deqs->tail->state = ENV;
	else if (keyval.mid)
		deqs->tail->state = EXPORT;
	return (EXIT_SUCCESS);
}