/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pushswap.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 10:50:14 by sohykim           #+#    #+#             */
/*   Updated: 2024/05/08 19:33:26 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef DEQUE_H
# define DEQUE_H
# define INT_MAX 2147483647
# define INT_MIN -2147483648
# include "../../libft/libft.h"
# include "../../ft_err.h"

typedef enum e_status
{
	NO_EXPORT = 0,
	EXPORT = 1,
	ENV = 2
}	t_status;

typedef struct s_deque
{
	char			*key;
	char			mid;
	char			*val;
	t_status		state;
	struct s_deque	*prev;
	struct s_deque	*next;
}	t_deque;

typedef struct s_deques
{
	t_deque	*head;
	t_deque	*tail;
}	t_deques;

t_deques	*create_deques(void);
t_deque		*create_deque(void);
t_deque		*pop_front(t_deques *deques);
t_deque		*pop_back(t_deques *deques);
int			set_env(t_deque *deq, char *keyval);
t_deques	*strstodeq(char **strs);
char		**deqtoenvp(t_deques *deqs);
int			push_back(t_deques *deques);
int			push_front(t_deques *deques);
void		free_deque(t_deque *deque);
void		free_deques(t_deques **deques);
void		free_strs(char **strs);

#endif