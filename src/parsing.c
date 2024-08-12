#include "../minishell.h"
char *find_pipeend(char *buffer);

/* utils */
int	ismadeofchr(char *str, char chr)
{
	while (str && *str)
	{
		if (*str != chr)
			return (FALSE);
		str++;
	}
	return (TRUE);
}

char	*ft_substrjoin (char *str, int start, int len, char *dst)
{
	char	*olddst;
	char	*src;

	olddst = dst;
	src = ft_substr(str, start, len);
	if (!src)
	{
		free(olddst);
		return (NULL);
	}
	dst = ft_strjoin(dst, src);
	if (!dst)
	{
		free(olddst);
		free(src);
		return (NULL);
	}
	free(olddst);
	free(src);
	return (dst);
}

/* quote */
int	wordlen(char *str, int space_opt)
{
	char	*wordend;

	wordend = find_wordend(str, space_opt);
	if (!*wordend)
		return (wordend - str); //wordend가 널문자 일 때 그대로 반환
	return (wordend - str + 1);  //아닐 때는 널문자 자리 + 1 해주고 반환 
}

/* token lst */
int	add_token(t_token *token, int type, char *word, char **argvs)
{
	t_token *newtoken;

	if (set_token(&newtoken) == EXTRA_ERROR)
	{	
		free(word);
		free(argvs);
		return (EXTRA_ERROR);
	}
	newtoken->type = type;
	newtoken->word = word;
	newtoken->argvs = argvs;
	tlst_addright(&token, newtoken);
	return (EXIT_SUCCESS);
}

/* pipe.c */
int	count_pipe(char *buffer)
{
	int	n;

	n = 1; //buffer가 비어있어도 pipe는 무조건 1개 생성
	while (*buffer)
	{	
		buffer += wordlen(buffer, PIPE); // |의 개수 
		if (*buffer == PIPE)
			n++;
		buffer++;
	}
	return (n);
}

int handle_empty_pipe(char *buffer)
{
	int		pipe_open;

	pipe_open = 0;
	while(*buffer)
	{
		if (*buffer == PIPE)
			pipe_open++;
		else if (pipe_open && *buffer != PIPE && !ft_isspace(*buffer))
			pipe_open--;
		if (pipe_open == 2)
		{
			*(buffer + 1) = '\0';
			status = handle_error(NULL, buffer, SYN_TOK);
			return (status);
		}
		buffer++;
	}
	return (EXIT_SUCCESS);

}

int ispipeopen(char *buffer)
{
	int pipe_open;

	pipe_open = 0;
	while(*buffer)
	{
		if (*buffer == PIPE)
			pipe_open++;
		else if (pipe_open && *buffer != PIPE && !ft_isspace(*buffer))
			pipe_open--;
		buffer++;
	}
	if (pipe_open)
		return (TRUE);
	return (FALSE);
}

char *get_pipeline(char *buffer)
{
	char *dstbuffer;
	char *srcbuffer;

	srcbuffer = readline(">");
	if (!srcbuffer)
		dstbuffer = NULL;
	else
	{
		dstbuffer = ft_strjoin(buffer, srcbuffer);
		free(srcbuffer);
		if (!dstbuffer)
			dstbuffer = NULL;
	}
	free(buffer);
	return (dstbuffer);
}

char **split_pipe(char *buffer)
{
	char	**strs;
	int		pipe_num;
	int		strlen;
	int		index;

	pipe_num = count_pipe(buffer);
	strs = ft_calloc(pipe_num + 1, sizeof(char *));
	if (!strs)
		return (NULL);
	index = 0;
	while (index < pipe_num)
	{
		while (ft_isspace(*buffer))
			buffer++;
		strlen = find_pipeend(buffer) - buffer + 1;
		strs[index] = ft_substr(buffer, 0, strlen);
		if (!strs[index])
		{
			free_strs(strs);
			return (NULL);
		}
		index++;
		buffer += strlen;
	}
	return (strs);
}

/* valid_buffer */
char *get_valid_buffer(char *headbuffer)
{
	char *vbuffer;

	vbuffer = ft_strdup(headbuffer);
	while(1)
	{
		if (handle_empty_pipe(vbuffer) == SYNTAX_ERROR) //비어있는 pipe 에러처리
			return (NULL);
		else if (ispipeopen(vbuffer) == TRUE) //열린 pipe 확인
		{
			vbuffer = get_pipeline(vbuffer); //추가입력 받기 
			if (!vbuffer) //입력 term 종료 했을 때, 메모리 부족할 때
			{
				free(headbuffer);
				status = handle_error(NULL, NULL, SYN_TERM);
				return (NULL);
			}
			else if (!*vbuffer)
			{
				return (NULL);
			}
		}
		else
			break;
	}
	free(headbuffer);
	return (vbuffer);
}

/* resword.c */
int ft_isredirect(char chr)
{
	if (chr == LESS || chr == GREAT)
		return (TRUE);
	return (FALSE);
}

int ft_ispipe(char chr)
{
	if (chr == PIPE)
		return (TRUE);
	return (FALSE);
}

int ft_isresword(char chr)
{
	if (ft_isredirect(chr) || ft_ispipe(chr))
		return (TRUE);
	return (FALSE);
}

/*find.c*/
char *find_redirect_start(char *str)
{
	while (*str && !ft_isredirect(*str))
		str++;
	return (str);
}

char *find_filename_start(char *str)
{
	while (ft_isspace(*str))
		str++;
	if (*str == LESS && *(str + 1) == LESS)
		str += 2;
	else if (*str == GREAT && *(str + 1) == GREAT)
		str += 2;
	else if (*str == LESS || *str == GREAT)
		str++;
	while (ft_isspace(*str))
		str++;
	return (str);
}

char *find_pipeend(char *buffer)
{
	while (buffer && *buffer)
	{	
		if (*buffer == SGL_QUOTE || *buffer == DBL_QUOTE)
		{
			buffer = find_wordend(buffer, 0);
			if (!buffer)
				return (buffer);
		}
		else if (*buffer == PIPE)
			return (buffer);
		buffer++;
	}
	return (NULL);
}

/* handle empty */
int handle_empty_redirect(char *str)
{
	str = find_filename_start(str);
	if (ft_isresword(*str) == TRUE)
	{	
		*(str + 1) = '\0';
		status = handle_error(NULL, str, SYN_TOK);
		return(1);
	}
	return(0);
}

int handle_empty_redirects(char **strs)
{
	while (strs && *strs)
	{
		if (handle_empty_redirect(*strs) == 1)
			return (EXIT_FAILURE);
		strs++;
	}
	return (EXIT_SUCCESS);
}

/* trim.c */
char *trim_space(char *str)
{
	char	*old_str;

	if (ismadeofchr(str, SPACE) == TRUE)
	{
		old_str = str;
		str = ft_substr(" ", 0, 1);
		free (old_str);
		if (!str)
			return (NULL);
	}
	else if (ft_strchr(str, SPACE) || ft_strchr(str, PIPE))
	{
		old_str = str;
		str = ft_strtrim(str, " |");
		free (old_str);
		if (!str)
			return (NULL);
	}
	return (str);
}

char *trim_quote(char *str)
{
	char	*old_str;
	char 	*new_str;
	int		len;

	old_str = str;
	new_str = NULL;
	while (*str)
	{
		len = wordlen(str, SPACE);
		if ((*str == SGL_QUOTE || *str == DBL_QUOTE))
		{
			new_str = ft_substr(str + 1, 0, len - 2);
			free(old_str);
			if (!new_str)
				return (NULL);
			break;
		}
		else 
		{
			new_str = old_str;
			break;
		}
		str += len;
	}
	return (new_str);
}

/* read */
int read_redirect_typeno(char *str)
{
	if (*str == LESS && *(str + 1) == LESS)
		return(T_DLESS);
	else if (*str == GREAT && *(str + 1) == GREAT)
		return(T_DGREAT);
	else if (*str == LESS)
		return(T_LESS);
	else if (*str == GREAT)
		return(T_GREAT);
	return (EXIT_SUCCESS);
}

/* replace.c */

int replace_pcode(char **str, char **envp)
{
	if (**str && **str == DOLLAR)
	{
		(*str)++;
		if (**str && **str == '?')
		{
			*envp = ft_itoa(status);
			if (!*envp)
				return (EXTRA_ERROR);
			(*str)++;
			return (TRUE);
		}
	}
	return (FALSE);
}

int replace_val_env(char **str, char **envp, t_deques *deqs)
{
	char	*key; 

	key = ft_substr(*str, 0, ft_strchr(*str, ' ') - *str);
	if (!key)
		return (EXTRA_ERROR);
	*envp = get_val_deq(deqs, key);
	if (!envp)
	{
		free(key);
		return (EXTRA_ERROR);
	}
	(*str) += (ft_strlen(key) - 1);
	free(key);
	return (EXIT_SUCCESS);
}

char *replace_dollar(t_deques *deqs, char **str, char *dst)
{
	char	*old_dst;
	char	*envp;

	int		errno;

	errno = replace_pcode(str, &envp);
	if (errno == EXTRA_ERROR)
		return (NULL);
	else if (errno == TRUE)
		return (envp);
	if (replace_val_env(str, &envp, deqs) == EXTRA_ERROR)
		return (NULL);
	old_dst = dst;
	dst = ft_strjoin(dst, envp);
	free(old_dst);
	free(envp);
	return (dst);
}

// char *find_dollar_end()
// {
// 	if (ft_strchr(str, DOLLAR))
// 		len = ft_strchr(str, DOLLAR) - str;
// 	else 
// 		len = ft_strlen(str);
// }

char *replace_envp(t_deques *deqs, char *str)
{
	char	*headstr;
	char	*dst;
	int		len;
	
	dst = ft_calloc(1, sizeof(char));
	if (!ft_strchr(str, DOLLAR))
		return (str);
	headstr = str;
	while (str && *str)
	{
		len = 0;
		if (*str == DOLLAR)
			dst = replace_dollar(deqs, &str, dst);
		else
		{
			if (ft_strchr(str, DOLLAR))
				len = ft_strchr(str, DOLLAR) - str;
			else 
				len = ft_strlen(str);
			dst = ft_substrjoin(str, 0, len, dst);
			str += len;
		}
		str++;
	}
	free(headstr);
	return (dst);
}


char *replace_word(t_deques *deqs, char *str)
{
	char *new_str;

	new_str = trim_space(str);
	// printf("new_str1: %s#\n", new_str);
	new_str = replace_envp(deqs, new_str);
	// printf("new_str2: %s#\n", new_str);
	new_str = trim_quote(new_str);
	// printf("new_str3: %s#\n", new_str);
	return (new_str);
}

char *replace_filename(char *str, int typeno)
{
	char	*filename;
	int		filenamelen;

	filename = NULL;
	if (typeno >= T_DLESS && typeno <= T_GREAT)
	{	
		filenamelen = wordlen(str, SPACE);
		filename = ft_substr(str, 0, filenamelen);
		if (!filename) // ft_substr d
		{
			//free
			handle_error(NULL, NULL, EXTRA_ERROR);
			return (NULL);
		}
		if (*filename == '\0')
		{
			status = handle_error(NULL, "newline", SYN_TOK);
			return (NULL);
		}
	}
	return (filename);
}

char *replace_words(char *dst_words, char *str, int len)
{
	char *src_words;
	char *head_words;

	if (len)
	{
		src_words = ft_substr(str, 0, len);
		head_words = dst_words;
		dst_words = ft_strjoin(dst_words, src_words);
		free(src_words);
		free(head_words);
	}
	return (dst_words);
}

/* argv.c */
int	count_argv(char *argv)
{
	int n;

	n = 0;
	while (argv && *argv)
	{
		n++;
		argv += wordlen(argv, SPACE);
	}
	return (++n);
}	
char *find_spacend(char *str);

char **get_argvs(char *word, char *argv, t_deques *envps)
{ 
	char	**argvs;
	char	*new; 
	int		n; 
	int		i;
	int		len;

	argv = ft_ltrim(argv);
	n = count_argv(argv);
	argvs = ft_calloc(n + 1, sizeof(char *));
	argvs[0] = ft_strdup(word);
	i = 1;
	while (i < n && argv && *argv)
	{
		len = wordlen(argv, SPACE);
		new = ft_substr(argv, 0, len);
		argvs[i++] = replace_word(envps, new);
		argv += len;
	}
	return (argvs);
}

/* tokenizer */
int token_redirect(t_token **token, t_deques *envps, char *str)
{
	int		typeno;
	char	*filename;
	int		filenamelen;

	while(*str)
	{
		str = find_redirect_start(str);	// printf("redirect start: %s\n", str);
		typeno = read_redirect_typeno(str);
		str = find_filename_start(str);
		filename = replace_filename(str, typeno);
		if (!filename)
			return (status);
		if (typeno >= T_DLESS && typeno <= T_GREAT)
		{
			filenamelen = wordlen(str, SPACE);
			filename = replace_word(envps, filename);
			add_token(*token, typeno, filename, NULL);
			str += filenamelen;
		}
		else
			str++;
	}
	return (EXIT_SUCCESS);
}

void token_word(t_token **token, t_deques *envps, char *str)
{
	char	*words;
	char	*word;
	char	*argv;
	int		len;
	char	**argvs; 

	argv = NULL;
	words = NULL;
	word = NULL;
	argvs = NULL;

	while (*str)
	{
		len = find_redirect_start(str) - str;
		if (len > 0)
			words = replace_words(words, str, len);
		str += len;
		if (*str)
		{
			str = find_filename_start(str);
			len = wordlen(str, SPACE);
			str += len;
		}
	}

	if (words && *words)
	{
		char *head = words;
		words = ft_ltrim(words);
		len = wordlen(words, SPACE);
		word = ft_substr(words, 0, len);
		word = replace_word(envps, word);
		argv = ft_substr(words + len, 0, ft_strlen(words + len));
		argvs = get_argvs(word, argv, envps);
		free(head);
		free(argv);
	}
	else
	{
		word = ft_calloc(1, sizeof(char));
		argvs = ft_calloc(1, sizeof(char*));
	}
	add_token(*token, T_CMD_WORD, word, argvs);

}

void token_pipe(t_token **token)
{
	add_token(*token, T_PIPE, NULL, NULL);
}

t_token *tokenizer(t_deques *envps, char **strs)
{
	t_token *token;
	(void) envps;
	//예외처리
	if (handle_empty_redirects(strs) == EXIT_FAILURE)
		return (NULL);
	//token
	while (strs && *strs)
	{	
		token_pipe(&token);
		if (token_redirect(&token, envps, *strs) == SYNTAX_ERROR)
			return (NULL);
		token_word(&token, envps, *strs);
		
		strs++;

	}
	/*token 출력*/
	// debug_token(token);
	return (token);
}

/* lexer */
void	lexer(t_shell *shell, char *vbuffer)
{
	(void) shell;
	char	**vbuffers;

	vbuffers = split_pipe(vbuffer);
	free(vbuffer);
	if (!vbuffers)
		return ;
	// /*buffers 출력*/
	// debug_buffers(vbuffers);

	shell->t = ft_calloc(1, sizeof(t_token));

	shell->t = tokenizer(shell->data.envps, vbuffers); //leaks

	free_strs(vbuffers);

	if (!shell->t)
		return ;
}

/* parslines.c */
void	parselines(t_shell *shell, char *buffer)
{
	char *vbuffer;

	// printf("buffer: %s\n", buffer);
	vbuffer = get_valid_buffer(buffer);
	// printf("vbuffer: %s\n", buffer);
	// free (buffer);
	if (!vbuffer)
		return ;
	lexer(shell, vbuffer);
	
	parser(&shell->t);

}
