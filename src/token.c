/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 21:47:20 by yubin             #+#    #+#             */
/*   Updated: 2024/08/20 11:53:10 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	token_pipe(t_token **token);
int	token_redirect(t_token **token, t_deques *envps, char *buffer);
int	token_word(t_token **token, t_deques *envps, char *str);

int	tokenizer(t_token **token, t_deques *envps, char **strs)
{
	// t_token	*token;

	*token = ft_calloc(1, sizeof(t_token));
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

int token_redirect(t_token **token, t_deques *envps, char *buffer)
{
	char	*filename;
	int		typeno;
	int		len; 

	while (*buffer)
	{
		buffer = find_redirect_head(buffer); // 
		if (!*buffer)
			return (EXIT_SUCCESS);
		typeno = read_redirect_typeno(buffer);
		len = 0;
		buffer = wordlen_filename(buffer, &len);
		if (!*buffer)
			return (EXIT_SUCCESS);
		filename = ft_substr(buffer, 0, len);
		// printf("filename:%s\n", filename);
		if (!filename)
			return (EXTRA_ERROR);
		filename = replace_word(envps, filename);
		if (add_tokenright(token, typeno, filename, NULL) == EXTRA_ERROR)
			return (EXTRA_ERROR);
		buffer += len;
	}
	return (EXIT_SUCCESS);
}

int token_word(t_token **token, t_deques *envps, char *str)
{
	char	*headwords;
	char	*words;
	char	*word;
	char	**argvs; 
	int		len;

	word = NULL;
	argvs = NULL;
	words = get_words(str);
	if (words)
	{
		headwords = words;
		len = 0;
		words = wordlen_word(words, &len);
		word = substrjoin(word, words, len);
		word = replace_word(envps, word);
		// printf("=====word complete=====\n");
		// if (word)
		argvs = get_argvs(envps, words);
		// printf("=====argvs complete=====\n");
		free(headwords);
	}
	// printf("argvs:%s\n", argvs[0]);
	if (add_tokenright_words(token, word, argvs) == EXTRA_ERROR)
		return (EXTRA_ERROR);
	return (EXIT_SUCCESS);
}
