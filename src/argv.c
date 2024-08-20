/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argv.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 22:36:14 by yubin             #+#    #+#             */
/*   Updated: 2024/08/20 16:31:02 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	init_argvs(char ***argvs, char *words);
int	count_argv(char *words);

int	get_words(char **words, char *str)
{
	int		len;

	*words = NULL;
	while (*str)
	{
		len = find_redirect_head(str) - str;
		*words = substrjoin_free(*words, str, len);
		if (*words == NULL)
			return (EXTRA_ERROR);
		str += len;
		str = wordlen_filename(str, &len);
		str += len;
	}
	if (!*words)
		*words = ft_strdup(str);
	return (EXIT_SUCCESS);
}

int	get_word(t_deques *envps, char **word, char *words)
{
	int		len;

	words = wordlen_word(words, &len);
	*word = substrjoin_free(*word, words, len);
	if (*word == NULL)
		return (EXTRA_ERROR);
	*word = replace_word(envps, *word);
	if (*word == NULL)
		return (EXTRA_ERROR);
	return (EXIT_SUCCESS);
}

int	get_argvs(t_deques *envps, char ***argvs, char *words)
{
	char	*argv;
	int		len;
	int		index;

	if (init_argvs(argvs, words) == EXTRA_ERROR)
		return (EXTRA_ERROR);
	index = 0;
	while (words && *words)
	{
		words = find_notspace(words);
		words = wordlen_word(words, &len);
		argv = ft_substr(words, 0, len);
		if (argv)
			argv = replace_word(envps, argv);
		if (!argv)
			return (EXTRA_ERROR);
		if (*argv)
			(*argvs)[index++] = argv;
		else
			free(argv);
		words += len;
	}
	if (!**argvs)
		**argvs = ft_strdup(words);
	return (EXIT_SUCCESS);
}

int	init_argvs(char ***argvs, char *words)
{
	int	argvnums;

	*argvs = NULL;
	argvnums = count_argv(words);
	if (argvnums == 0)
		argvnums = 1;
	*argvs = ft_calloc(argvnums + 1, sizeof(char *));
	if (!*argvs)
		return (EXTRA_ERROR);
	return (EXIT_SUCCESS);
}

int	count_argv(char *words)
{
	char	*word;
	int		len;
	int		argvnums;

	argvnums = 0;
	while (*words)
	{
		word = wordlen_word(words, &len);
		words += len;
		words = find_notspace(words);
		argvnums++;
	}
	return (argvnums);
}	
