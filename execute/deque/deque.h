/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 19:33:19 by sohykim           #+#    #+#             */
/*   Updated: 2024/07/29 15:49:48 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEQUE_H
# define DEQUE_H
# include "../../libft/libft.h"
# include "../../ft_err.h"

typedef enum e_state
{
	NO = 0,
	EXPORT = 1,
	ENV = 2
}	t_state;

typedef struct s_map
{
	char	*key;
	char	mid;
	char	*val;
}	t_map;

typedef struct s_deque
{
	t_map			keyval;
	t_state			state;
	struct s_deque	*prev;
	struct s_deque	*next;
}	t_deque;

typedef struct s_deques
{
	t_deque	*head;
	t_deque	*tail;
}	t_deques;

/*deque.c*/
t_deques	*create_deques(void);
t_deque		*create_deque(void);

/*push.c*/
int			push_back(t_deques *deques, t_map keyval);
int			replace_back(t_deques *deqs, char *key, char mid, char *val);
int			push_keyval(t_deques *deqs, char *key, char mid, char *val);

/*free.c*/
void		free_deque(t_deque *deque);
void		free_deques(t_deques **deques);
void		free_strs(char **strs);
void		free_map(t_map *keyval);
void		remove_targetdeq(t_deques *deqs, t_deque *target);

/*pop.c*/
t_deque		*pop_front(t_deques *deques);
t_deque		*pop_back(t_deques *deques);
t_deque		*pop_target(t_deques *deqs, t_deque *target);

/*api.c*/
char		*read_val_deq(t_deques *deq, char *key);
char		**deqtostrs(t_deque *deq);
t_deques	*strstodeq(char **strs);
char		*get_val_deq(t_deques *deqs, char *key);

/*utils.c*/
t_deque		*find_deq(t_deques *deq, char *key);
int			is_valid_key(char *s);
void		set_state(t_deque *deq);

/*map.c*/
char		*ft_pairjoin(char *key, char mid, char *val);
char		*get_key(char *str);
int			set_map(t_map *keyval, char *str);
int			set_keyval(t_map *keyval, char *key, char mid, char *val);

#endif
