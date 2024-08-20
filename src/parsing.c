#include "../minishell.h"

//릭 확인 & 리팩토링 

//replace함수: 할당한 문자열을 넘겨주면 문자열이 대체 될 때 기존 문자열을 free해주는 함수들 

//echo a || " || " || echo c
//	  echo	 a   |   "  echo   |  " b	|	echo   "c"

char *replace_quote(char *str)
{
	char	*headstr;
	char 	*dst;
	char 	*tmp;
	char 	*src;
	int		len;

	headstr = str;
	dst = ft_calloc(1, sizeof(char));
	while (*str)
	{
		len = wordlen(str);
		// printf("len:%d\n", len);
		if (ft_isquotend(str, ft_isquote(*str)) == EXIT_SUCCESS)
		{
			// printf("str:%s\n", str);
			src = ft_substr(str, 1, len - 2);
		}
		else
			src = ft_substr(str, 0, len);
		tmp = dst;
		dst = ft_strjoin(tmp, src);
		free(tmp);
		// printf("dst:%s\n", dst);
		str += len;
	}
	free (headstr);

	// printf("dst:%s\n", dst);
	return (dst);
}

char *replace_word(t_deques *envps, char *str)
{
	str = replace_value(envps, str);
	if (!str)
		return (NULL);
	// printf("new_str1:%s#\n", str);
	str = replace_quote(str);
	if (!str)
		return (NULL);
	// printf("new_str2:%s#\n", str);
	return (str);
}

/* lexer */
int	lexer(t_token **token, t_deques *envps, char *buffer)
{
	(void) envps;
	(void) token;
	char	**buffers;

	buffers = split_pipe(buffer); //refatoring complete
	if (!buffers)
		return (EXTRA_ERROR);
	/*buffers 출력*/
	// debug_buffers(buffers); 
	tokenizer(token, envps, buffers); //leaks
	free_strs(buffers);
	// if (!*token)
	// 	return (EXTRA_ERROR);
	// printf("===tokenizer complete===\n");
	return (EXIT_SUCCESS);
}

/* parslines.c */
int	parselines(t_shell *shell, char *buffer)
{
	char	*validbuffer;
	int		code;

	validbuffer = NULL;
	// printf("buffer:%s\n", buffer);
	code = get_validbuffer(buffer, &validbuffer);
	free(buffer);
	if (code == EXTRA_ERROR || code == SYNTAX_ERROR)
		return (EXIT_FAILURE);
	// printf("validbuffer:%s\n", validbuffer);
	// printf("===validbuffer complete===\n");
	if (lexer(&(shell->t), shell->data.envps, validbuffer) == EXTRA_ERROR)
		return (EXIT_FAILURE);
	free(validbuffer);
	// printf("===lexer complete===\n");
	parser(&shell->t);
	return (EXIT_SUCCESS);
}
