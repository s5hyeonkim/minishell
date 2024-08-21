/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_lst.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 16:04:39 by yubshin           #+#    #+#             */
/*   Updated: 2024/08/21 14:01:23 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_token	*tlst_lastright(t_token *t)
{
	if (t == NULL)
		return (t);
	while (t->right)
		t = t->right;
	return (t);
}

t_token	*tlst_lastleft(t_token *t)
{
	if (t == NULL)
		return (NULL);
	while (t->left != NULL)
		t = t->left;
	return (t);
}

void	tlst_addright(t_token **t, t_token *newtoken)
{
	t_token	*lasttoken;

	if (*t == NULL)
		*t = newtoken;
	else
	{
		lasttoken = tlst_lastright(*t);
		lasttoken->right = newtoken;
	}
}

void	tlst_addleft(t_token **t, t_token *newtoken)
{
	t_token	*lasttoken;

	if (*t == NULL)
		*t = newtoken;
	else if (newtoken != NULL)
	{
		lasttoken = tlst_lastleft(*t);
		lasttoken->left = newtoken;
	}
}
