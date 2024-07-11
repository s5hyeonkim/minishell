/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creation.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 12:49:55 by sohykim           #+#    #+#             */
/*   Updated: 2024/07/11 12:49:57 by sohykim          ###   ########.fr       */
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
