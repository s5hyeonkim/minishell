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

char *substr_free(char *str, unsigned int start, size_t len)
{
	char *dst;

	dst = ft_substr(str, start, len);
	if (str)
		free(str);
	if (!dst)
		return (NULL);
	return (dst);
}


char *strjoin_free(char *str1, char *str2)
{
	char *dst;

	dst = ft_strjoin(str1, str2);
	if (str1)
		free(str1);
	if (str2)
		free(str2);
	if (!dst)
		return (NULL);
	return (dst);
}


char	*substrjoin (char *str, int start, int len, char *dst)
{
	char	*src;

	src = substr_free(str, start, len);
	if (!src)
		return (NULL);
	dst = strjoin_free(dst, src);
	if (!dst)
		return (NULL);
	return (dst);
}

/* quote */
int	wordlen(char *str, int space_opt)
{
	char	*wordend;

	wordend = find_wordend(str, space_opt);
	if (!wordend)
		return (0);
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
	{	
		free(buffer);
		dstbuffer = NULL;
	}
	else
	{
		dstbuffer = strjoin_free(buffer, srcbuffer);
		// dstbuffer = ft_strjoin(buffer, srcbuffer);
		// free(srcbuffer);
		if (!dstbuffer)
			dstbuffer = NULL;
	}
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
				return (NULL);
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
		if ((*str == SGL_QUOTE || *str == DBL_QUOTE) && len)
		{
			// new_str = substr_free(str, 1 , len - 2);
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

char *replace_pcode(char *str)
{
	char *envp; 

	envp = NULL;
	if (*str== '?')
	{
		envp = ft_itoa(status);
		if (!envp)
			return (NULL);
		(str)++;
	}
	return (envp);
}

char *replace_val_env(t_deques *deqs, char *key)
{
	char	*envp;

	envp = get_val_deq(deqs, key);
	if (!envp)
		return (NULL);
	return (envp);
}


int strchrlen(char *str, int chr)
{
	int len ;

	if (ft_strchr(str, chr))
		len = ft_strchr(str, chr) - str;
	else 
		len = ft_strlen(str);
	return (len);
}

char *get_key2(char *str)
{
	char	*key; 
	int 	len;

	len = strchrlen(str, SPACE);
	key = ft_substr(str, 0, len);
	if (!key)
		return (NULL);
	return (key);
}

char *replace_dollar(t_deques *deqs, char **str, char *dst)
{
	char	*envp;
	int		len;
	char 	*key;

	(*str)++;
	envp = replace_pcode(*str);
	if (envp)
		(*str)++;
	else
	{
		key = get_key2(*str);
		// printf("key: %s\n", key);
		if (!key)
			return (NULL);
		envp = replace_val_env(deqs, key);
		len = ft_strlen(key) - 1;
		free(key);
		(*str) += len;
	}

	dst = strjoin_free(dst, envp);
	if (!dst)
		return (NULL);
	// printf("dst: %s\n", dst);
	return (dst);
}


char *replace_envp(t_deques *deqs, char *str)
{
	char	*headstr;
	char	*dst;
	int		len;
	
	dst = ft_calloc(1, sizeof(char));
	if (!dst)
		return (NULL);
	headstr = str;
	while (*str)
	{
		// printf("str: %s\n", str);
		if (*str == DOLLAR)
			dst = replace_dollar(deqs, &str, dst);
		else
		{
			len = strchrlen(str, DOLLAR);
			dst = substrjoin(str, 0, len, dst);
			str += len;
		}
		if (!dst || !*str)
			break ;
		str++;
	}
	free(headstr);
	// printf(">dst: %s\n", dst);
	return (dst);
}

char *replace_word(t_deques *deqs, char *str)
{
	char *new_str;

	// printf("start\n");
	new_str = trim_space(str);
	// printf("new_str1: %s#\n", new_str);
	if (ft_strchr(str, DOLLAR))
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

	if (len)
	{
		src_words = ft_substr(str, 0, len);
		if (!src_words)
			return (NULL);
		dst_words = strjoin_free(dst_words, src_words);
		if (!dst_words)
			return (NULL);
	}
	return (dst_words);
}

/* argv.c */
int	count_argv(char *argv)
{
	int n;
	int len;

	n = 0;
	while (argv && *argv)
	{
		n++;
		len = wordlen(argv, SPACE);
		if (len)
			argv += len;
		else
			break ;
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
	if (word)
		argvs[0] = ft_strdup(word);
	i = 1;
	while (i < n && argv && *argv)
	{
		argv = find_spacend(argv);
		if (*(argv + 1) == PIPE)
			argv++;
		len = wordlen(argv, SPACE);
		// printf("len: %d\n", len);
		if (!len)
			len = ft_strlen(argv);
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


char *get_words(char *str)
{
	char	*words;
	int		len;

	words = NULL;
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
	return (words);
}

int add_tokenwords(t_token **token, char *word, char **argvs)
{
	if (!word)
	{
		word = ft_calloc(1, sizeof(char));
		if (!word)
			return (EXTRA_ERROR);
	}
	if (!argvs)
	{
		argvs = ft_calloc(1, sizeof(char*));
		if (!argvs)
			return (EXTRA_ERROR);
	}
	if (add_token(*token, T_CMD_WORD, word, argvs) == EXTRA_ERROR)
		return (EXTRA_ERROR);
	return (EXIT_SUCCESS);
}


char *get_word(t_deques *envps, char *words)
{
	char	*word;
	int		len;

	words = ft_ltrim(words);
	len = wordlen(words, SPACE);
	word = ft_substr(words, 0, len);
	if (!word)
		return (NULL);
	word = replace_word(envps, word);
	return (word);
}

char **get_argvs_center(t_deques *envps, char *words, char *word)
{
	char	*argv;
	char	**argvs; 
	int		len;

	words = ft_ltrim(words);
	len = wordlen(words, SPACE);
	argv = ft_substr(words + len, 0, ft_strlen(words + len));
	if (!argv)
		return (NULL);
	argvs = get_argvs(word, argv, envps);
	free(argv);
	if (!argvs)
		return (NULL);
	return (argvs);
}

int token_word(t_token **token, t_deques *envps, char *str)
{
	char	*headwords;
	char	*words;
	char	*word;
	char	**argvs; 

	word = NULL;
	argvs = NULL;
	words = get_words(str);
	if (words)
	{
		headwords = words;
		word = get_word(envps, words);
		if (word)
			argvs = get_argvs_center(envps, words, word);
		free(headwords);
	}
	if (add_tokenwords(token, word, argvs) == EXTRA_ERROR)
		return (EXTRA_ERROR);
	return (EXIT_SUCCESS);
}

void token_pipe(t_token **token)
{
	add_token(*token, T_PIPE, NULL, NULL);
}

t_token *tokenizer(t_deques *envps, char **strs)
{
	t_token *token;

	if (handle_empty_redirects(strs) == EXIT_FAILURE)
		return (NULL);
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
int parselines(t_shell *shell, char *buffer)
{
	char *vbuffer;

	// printf("buffer: %s\n", buffer);
	vbuffer = get_valid_buffer(buffer);
	// printf("vbuffer: %s\n", buffer);
	// free (buffer);
	if (!vbuffer)
		return (EXIT_FAILURE);
	lexer(shell, vbuffer);
	
	parser(&shell->t);
	return (EXIT_SUCCESS);
}
