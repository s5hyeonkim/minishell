/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 16:36:13 by yubshin           #+#    #+#             */
/*   Updated: 2024/08/20 16:36:21 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* terminal */
void	get_terminal(t_shell *shell)
{
	tcgetattr(1, &shell->term);
}

void	reset_terminal(t_shell *shell)
{
	tcsetattr(1, 0, &shell->term);
}
