/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 21:47:20 by yubin             #+#    #+#             */
/*   Updated: 2024/08/20 16:38:09 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	token_pipe(t_token **token);
int	token_redirect(t_token **token, t_deques *envps, char *buffer);
int	token_word(t_token **token, t_deques *envps, char *str);

int	tokenizer(t_token **token, t_deques *envps, char **strs)
{
	*token = ft_calloc(1, sizeof (t_token));
	if (!*token)
		return (EXTRA_ERROR);
	while (*strs)
	{	
		if (token_pipe(token) == EXTRA_ERROR)
			return (EXTRA_ERROR);
		// printf("===token_pipe :complete===\n");
		if (token_redirect(token, envps, *strs) == EXTRA_ERROR)
			return (EXTRA_ERROR);
		// printf("===token_redirect : complete===\n");
		if (token_word(token, envps, *strs) == EXTRA_ERROR)
			return (EXTRA_ERROR);
		// printf("===token_word :complete===\n");
		strs++;
	}
	/*token 출력*/
	// debug_token(token);
	return (EXIT_SUCCESS);
}

int	token_pipe(t_token **token)
{
	if (add_tokenright(token, T_PIPE, NULL, NULL) == EXTRA_ERROR)
		return (EXTRA_ERROR);
	return (EXIT_SUCCESS);
}

int	token_redirect(t_token **token, t_deques *envps, char *buffer)
{
	char	*filename;
	int		typeno;
	int		len;

	while (*buffer)
	{
		buffer = find_redirect_head(buffer);
		if (*buffer == '\0')
			return (EXIT_SUCCESS);
		typeno = read_redirect_typeno(buffer);
		len = 0;
		buffer = wordlen_filename(buffer, &len);
		if (*buffer == '\0')
			return (EXIT_SUCCESS);
		filename = ft_substr(buffer, 0, len);
		if (filename == NULL)
			return (EXTRA_ERROR);
		filename = replace_word(envps, filename);
		if (add_tokenright(token, typeno, filename, NULL) == EXTRA_ERROR)
			return (EXTRA_ERROR);
		buffer += len;
	}
	return (EXIT_SUCCESS);
}

int	token_word(t_token **token, t_deques *envps, char *str)
{
	char	*words;
	char	*word;
	char	**argvs;
	char	*headwords;

	words = NULL;
	word = NULL;
	argvs = NULL;
	if (get_words(&words, str) == EXTRA_ERROR)
		return (EXTRA_ERROR);
	if (words)
	{
		headwords = words;
		if (get_word(envps, &word, words) == EXTRA_ERROR)
			return (EXTRA_ERROR);
		if (get_argvs(envps, &argvs, words) == EXTRA_ERROR)
			return (EXTRA_ERROR);
		free(headwords);
	}
	if (add_tokenright(token, T_CMD_WORD, word, argvs) == EXTRA_ERROR)
		return (EXTRA_ERROR);
	return (EXIT_SUCCESS);
}
