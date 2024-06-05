/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubin <yubin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 18:31:03 by yubin             #+#    #+#             */
/*   Updated: 2024/01/05 19:08:17 by yubin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <unistd.h>
# include <stdlib.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

# ifndef OPEN_MAX
#  define OPEN_MAX 10496
# endif

typedef struct s_list
{
	int				fd;
	int				readrt;
	int				mallen;
	char			*record;
	struct s_list	*next;
}					t_list;

char	*get_next_line(int fd);
int		ft_strchr(char *str, char chr);
int		ft_newadd_lst(t_list **rcdlst_ptr, t_list **fdlst_ptr, int fd);
int		ft_fdlst(t_list **rcdlst_ptr, t_list **fdlst_ptr, int fd);
int		ft_parse_rcd(t_list **fdlst_ptr, char **res_ptr, int reslen);
int		ft_strdup(t_list **rcdlst_ptr, t_list **fdlst_ptr, char **res_ptr);
void	ft_freelst(t_list **rcdlst_ptr, t_list **fdlst_ptr, char *res);
#endif
