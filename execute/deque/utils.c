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

static int	get_size(t_deque *head, t_status state)
{
	t_deque	*deq;
	int		size;

	deq = head;
	size = 0;
	while (TRUE)
	{
		if (deq->state >= state)
			size++;
		if (deq->next == head)
			break ;
		deq = deq->next;
	}
	return (size);
}

char	*ft_eqjoin(char *a, char *c)
{
	char	*temp;
	char	*ret;

	ret = NULL;
	temp = ft_strjoin(a, "=");
	if (!temp)
		return (NULL);
	ret = ft_strjoin(temp, c);
	free(temp);
	return (ret);
}

char	**deqtoenvp(t_deques *deqs)
{
	char	**ret;
	int		size;
	int		index;
	t_deque	*deq;

	size = get_size(deqs->head, ENV);
	ret = ft_calloc(size + 1, sizeof(char *));
	if (!ret)
		return (NULL);
	index = 0;
	deq = deqs->tail;
	while (index < size)
	{
		deq = deq->next;
		if (deq->state != ENV)
			continue ;
		ret[index] = ft_eqjoin(deq->key, deq->val);
		if (!ret[index++])
		{
			free_strs(ret);
			return (NULL);
		}
	}
	return (ret);
}

int	set_env(t_deque *deq, char *keyval)
{
	char	*p;

	p = ft_strchr(keyval, '=');
	if (p)
	{
		deq->key = ft_substr(keyval, 0, p - keyval + 1);
		if (!deq->key)
			return (MALLOC_FAILED);
		deq->mid = '=';
		deq->val = ft_strdup(p + 1);
		if (!deq->val)
			return (MALLOC_FAILED);
	}
	else
	{
		deq->key = ft_strdup(keyval);
		if (!deq->key)
			return (MALLOC_FAILED);
	}
	return (EXIT_SUCCESS);
}

t_deques	*strstodeq(char **strs)
{
	t_deques	*new;
	int			index;

	index = 0;
	new = create_deques();
	if (!new)
		return (NULL);
	while (strs[index])
	{
		if (push_back(new) || set_env(new->tail, strs[index]))
		{
			free_deques(&new);
			return (NULL);
		}
		new->tail->state = ENV;
		index++;
	}
	if (push_back(new) || set_env(new->tail, ""))
	{
		free_deques(&new);
		new = NULL;
	}
	return (new);
}