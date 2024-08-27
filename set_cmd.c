/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 11:39:03 by yubshin           #+#    #+#             */
/*   Updated: 2024/08/27 11:39:59 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	set_cmd(t_shell *shell)
{
	size_t	index;
	int		status;

	index = 0;
	if (set_env_paths(&shell->data))
		return (EXTRA_ERROR);
	while (index < shell->p_size)
	{
		if (set_args(&shell->p[index], shell->data))
			return (EXTRA_ERROR);
		status = set_redirect(&shell->p[index], shell->data.envps);
		if (status)
			return (status);
		index++;
	}
	return (EXIT_SUCCESS);
}
