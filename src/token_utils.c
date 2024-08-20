/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 10:44:45 by yubshin           #+#    #+#             */
/*   Updated: 2024/08/20 11:52:58 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	add_tokenright(t_token **token, int type, char *word, char **argvs);

int	add_tokenright_words(t_token **token, char *word, char **argvs)
{
	if (!word)
	{
		word = ft_calloc(2, sizeof(char));
		if (!word)
			return (EXTRA_ERROR);
	}
	if (!argvs)
	{
		argvs = ft_calloc(2, sizeof(char *));
		if (!argvs)
			return (EXTRA_ERROR);
	}	
	if (!*argvs)	
	{
		*argvs = ft_calloc(2, sizeof(char));
		if (!*argvs)
			return (EXTRA_ERROR);
	}
	// printf("argvs[0]:%s\n", argvs[0]);
	// printf("argvs[1]:%s\n", argvs[1]);
	if (add_tokenright(token, T_CMD_WORD, word, argvs) == EXTRA_ERROR)
		return (EXTRA_ERROR);
	return (EXIT_SUCCESS);
}

int	add_tokenright(t_token **token, int type, char *word, char **argvs)
{
	t_token *newtoken;

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
