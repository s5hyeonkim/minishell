/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argv.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 22:36:14 by yubin             #+#    #+#             */
/*   Updated: 2024/08/20 10:53:40 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int init_argvs(char ***argvs, char *words);
int	count_argv(char *words);

char *get_words(char *str)
{
	char	*words;
	int		len;

	words = NULL;
	if (!*str)
		words = ft_strdup(str);
	while (*str)
	{
		len = find_redirect_head(str) - str;
		if (len < 0)
		{
			len = ft_strlen(str);
			words = substrjoin(words, str, len);
			if (!words)
				return (NULL);
			break ;
		}
		if (len > 0)
		{
			words = substrjoin(words, str, len);
			if (!words)
				return (NULL);
		}
		str += len;
		len = 0;
		str = wordlen_filename(str, &len);
		str += len;
	}
	return (words);
}

char **get_argvs(t_deques *envps, char *words)
{
	char	**argvs;
	char	*argv;
	int		len;
	int		index;

	if (init_argvs(&argvs, words) == EXTRA_ERROR)
		return (NULL);
	index = 0;
	while (words && *words)
	{
		// printf("%s\n", words);
		words = find_notspace(words);
		if (!*words)
			break ;
		len = 0;
		words = wordlen_word(words, &len);
		argv = ft_substr(words, 0, len);
		if (argv)
			argv = replace_word(envps, argv);
		if (!argv)
			return(NULL);
		if (*argv)
			argvs[index++] = argv;
		else 
			free(argv);
		words += len;
	}
	// int j = 0;
	// while (argvs && argvs[j])
	// {
	// 	printf(">argvs[%d]%s\n", j, argvs[j]);
	// 	j++;
	// }
	return (argvs);
}

int init_argvs(char ***argvs, char *words)
{
	int argvnums;

	argvnums = count_argv(words);
	*argvs = ft_calloc(argvnums + 1, sizeof(char *));
	if (!*argvs)
		return (EXTRA_ERROR);
	return (EXIT_SUCCESS);
}

int	count_argv(char *words)
{
	char	*word;
	int		len;
	int argvnums;
	
	argvnums = 0;
	while (*words)
	{
		len = 0;
		word = wordlen_word(words, &len);
		words += len;
		words = find_notspace(words);
		// printf("len:%d\n", len);
		// if (len <= 0)
			// len = ft_strlen(word);
		argvnums++;
	}
	// printf("argvnums:%d\n", argvnums);
	return (argvnums);
}	
