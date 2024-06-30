/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_lifecycle.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 16:53:08 by sohykim           #+#    #+#             */
/*   Updated: 2024/05/23 18:56:02 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "deque.h"

t_deques	*create_deques(void)
{
	t_deques	*new_deques;

	new_deques = ft_calloc(1, sizeof(t_deques));
	return (new_deques);
}

t_deque	*create_deque(void)
{
	t_deque	*new_deque;

	new_deque = ft_calloc(1, sizeof(t_deque));
	return (new_deque);
}

void	free_strs(char **strs)
{
	size_t	index;

	index = 0;
	if (!strs)
		return ;
	while (strs[index])
		free(strs[index++]);
	free(strs);
}

void	free_deque(t_deque *deque)
{
	t_deque	*next;

	if (deque && deque->prev)
		deque->prev->next = NULL;
	while (deque)
	{
		next = deque->next;
		free(deque->val);
		free(deque->key);
		free(deque);
		deque = next;
	}
}

void	free_deques(t_deques **deques)
{
	if (*deques)
	{
		free_deque((*deques)->head);
		(*deques)->head = NULL;
		(*deques)->tail = NULL;
	}
	free(*deques);
	(*deques) = NULL;
}
