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

void	set_state(t_deque *deq)
{
	deq->state = EXPORT;
	if (deq->keyval.mid)
		deq->state = ENV;
}

int	set_deque(t_deque **deq, t_map keyval)
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

int	replace_back(t_deques *deqs, char *str)
{
	t_map	keyval;
	char	*key;

	key = get_key(str);
	if (!key)
		return (EXIT_FAILURE);
	if (find_deq(deqs, key))
		remove_targetdeq(deqs, find_deq(deqs, key));
	free(key);
	if (set_map(str, &keyval) || push_back(deqs, keyval))
	{
		free_map(&keyval);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	push_keyback(t_deques *deques, char *str)
{
	t_map	keyval;

	ft_memset(&keyval, 0, sizeof(t_map));
	keyval.key = ft_strdup(str);
	if (keyval.key)
		keyval.val = ft_strdup("");
	if (!keyval.key || !keyval.val)
	{
		free_map(&keyval);
		return (EXTRA_ERROR);
	}
	return (push_back(deques, keyval));
}
