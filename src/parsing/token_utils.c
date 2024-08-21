/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 10:44:45 by yubshin           #+#    #+#             */
/*   Updated: 2024/08/21 14:01:25 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	set_token(t_token **t)
{
	*t = ft_calloc(1, sizeof(t_token));
	if (*t == NULL)
		return (EXTRA_ERROR);
	return (EXIT_SUCCESS);
}

int	add_tokenright(t_token **token, int type, char *word, char **argvs)
{
	t_token	*newtoken;

	newtoken = ft_calloc(1, sizeof(t_token));
	if (!newtoken)
	{	
		free(word);
		free(argvs);
		return (EXTRA_ERROR);
	}
	newtoken->type = type;
	newtoken->word = word;
	newtoken->argvs = argvs;
	tlst_addright(token, newtoken);
	return (EXIT_SUCCESS);
}
