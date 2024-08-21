/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 14:31:30 by yubshin           #+#    #+#             */
/*   Updated: 2024/08/21 17:17:04 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int		syntax_pipeline(t_token **dsttoken, t_token *nowtoken);
int		syntax_simple_cmd(t_token **dsttoken, t_token *nowtoken);
int		syntax_cmd_redirects(t_token **dsttoken, t_token *nowtoken);
void	syntax_io_redirect(t_token **dsttoken, t_token *nowtoken);

int	parser(t_token **token)
{
	t_token	*nowtoken;
	t_token	*dsttoken;

	dsttoken = NULL;
	nowtoken = *token;
	if (syntax_pipeline(&dsttoken, nowtoken) == EXTRA_ERROR)
		return (EXTRA_ERROR);
	free(*token);
	*token = dsttoken;
	return (EXIT_SUCCESS);
}

int	syntax_pipeline(t_token **dsttoken, t_token *nowtoken)
{
	t_token	*nexttoken;
	t_token	*lasttoken;

	if (!nowtoken)
		return (EXIT_SUCCESS);
	nexttoken = nowtoken->right;
	if (nowtoken->type == T_PIPE)
	{	
		nowtoken->right = NULL;
		if (!*dsttoken)
			*dsttoken = nowtoken;
		else
			tlst_addright(dsttoken, nowtoken);
	}
	else
	{	
		lasttoken = tlst_lastright(*dsttoken);
		if (lasttoken)
			if (syntax_simple_cmd(&lasttoken->left, nowtoken) == EXTRA_ERROR)
				return (EXTRA_ERROR);
	}
	if (nexttoken)
		if (syntax_pipeline(dsttoken, nexttoken) == EXTRA_ERROR)
			return (EXTRA_ERROR);
	return (EXIT_SUCCESS);
}

int	syntax_simple_cmd(t_token **dsttoken, t_token *nowtoken)
{
	t_token	*newtoken;
	t_token	*nexttoken;

	if (!*dsttoken)
	{
		if (set_token(&newtoken) == EXTRA_ERROR)
			return (EXTRA_ERROR);
		newtoken->type = T_SIMPLE_CMD;
		*dsttoken = newtoken;
	}
	nexttoken = nowtoken->right;
	if (nowtoken && nowtoken->type == T_CMD_WORD)
	{
		nowtoken->right = NULL;
		tlst_addright(&(*dsttoken)->right, nowtoken);
	}
	else if (nowtoken)
		if (syntax_cmd_redirects(&(*dsttoken)->left, nowtoken) == EXTRA_ERROR)
			return (EXTRA_ERROR);
	return (EXIT_SUCCESS);
}	

int	syntax_cmd_redirects(t_token **dsttoken, t_token *nowtoken)
{
	t_token	*newtoken;
	t_token	*lasttoken;

	if (!nowtoken || nowtoken->type == T_PIPE)
		return (EXIT_SUCCESS);
	if (nowtoken && nowtoken->type >= T_DLESS && nowtoken->type <= T_GREAT)
	{	
		if (set_token(&newtoken) == EXTRA_ERROR)
			return (EXTRA_ERROR);
		newtoken->type = T_IO_REDIRECTS;
		if (!*dsttoken)
			*dsttoken = newtoken;
		else
			tlst_addright(dsttoken, newtoken);
		lasttoken = tlst_lastright(*dsttoken);
		syntax_io_redirect(&lasttoken, nowtoken);
	}
	return (EXIT_SUCCESS);
}

void	syntax_io_redirect(t_token **dsttoken, t_token *nowtoken)
{
	if (nowtoken && nowtoken->type >= T_DLESS && nowtoken->type <= T_GREAT)
	{	
		nowtoken->right = NULL;
		(*dsttoken)->left = nowtoken;
	}
}
