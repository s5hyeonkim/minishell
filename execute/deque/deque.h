/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 10:50:14 by sohykim           #+#    #+#             */
/*   Updated: 2024/07/05 12:36:38 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEQUE_H
# define DEQUE_H
# include "../../libft/libft.h"
# include "../../ft_err.h"

typedef enum e_status
{
	NO = 0,	// export와 관련 없는 것.
	EXPORT = 1,
	ENV = 2
}	t_status;

typedef struct s_pairs
{
	char	*key;
	char	mid; 	// = 또는 0
	char	*val; 	// val != NULL;
}	t_pairs;

typedef struct s_deque
{
	t_pairs			keyval;
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
char		**deqtoenvp(t_deques *deqs, t_status state);
int			push_back(t_deques *deques, t_pairs keyval);
int			push_front(t_deques *deques);
void		free_deque(t_deque *deque);
void		free_deques(t_deques **deques);
void		free_strs(char **strs);
void		free_keyval(t_pairs keyval);
char    	*get_key_str(char *str);
int			set_keyval(char *str, t_pairs *keyval);
char		*ft_pairjoin(char *key, char mid, char *val);
t_deque		*find_deq(t_deques *deq, char *key);
t_deque		*pop(t_deques *deqs, t_deque *target);
void		print_deques(t_deques *deq);
char		*read_val_deq(t_deques *deq, char *key);
int			push_keyval(t_deques *deqs, char *str);

#endif