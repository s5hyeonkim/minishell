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
void	set_state(t_deque *deq)
{
	if (deq->keyval.val[0])
		deq->state = ENV;
	else if (deq->keyval.mid)
		deq->state = EXPORT;
}

int	set_deque(t_deque **deq, t_pairs keyval)
{

	*deq = create_deque();
	if (*deq == NULL)
		return (EXTRA_ERROR);
	ft_memcpy(&((*deq)->keyval), &keyval, sizeof(t_pairs));
	set_state(*deq);
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

int	replace_back(t_deques *deqs, char *str)
{
	t_deque	*node;
	t_pairs	keyval;
	char	*key;

	key = get_key_str(str);
	if (!key)
		return (EXIT_FAILURE);
	node = pop(deqs, find_deq(deqs, key));
	free_deque(node);
	free(key);
	if (set_keyval(str, &keyval) || push_back(deqs, keyval))
	{
		free_keyval(keyval);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	push_keyback(t_deques *deques, char *str)
{
	t_pairs	keyval;

	ft_memset(&keyval, 0, sizeof(t_pairs));
	keyval.key = ft_strdup(str);
	if (keyval.key)
		keyval.val = ft_strdup("");
	if (!keyval.key || !keyval.val)
	{
		free(keyval.key);
		free(keyval.val);
		return (EXTRA_ERROR);
	}
	return (push_back(deques, keyval));
}