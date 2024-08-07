/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 14:44:13 by yubin             #+#    #+#             */
/*   Updated: 2024/07/29 15:50:32 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	free_str(char *str)
{
	free(str);
	str = NULL;
}

// void	free_strs(char **strs)
// {
// 	int	idx;

// 	idx = 0;
// 	while (strs && strs[idx])
// 	{
// 		free(strs[idx]);
// 		strs[idx] = 0;
// 		idx++;
// 	}
// 	free(strs);
// }