#include "../minishell.h"

char *find_pipeend(char *buffer);
char *find_spacend(char *str);


int strchrlen(char *str, int chr)
{
	int len ;

	if (ft_strchr(str, chr))
		len = ft_strchr(str, chr) - str;
	else 
		len = ft_strlen(str);
	return (len);
}

/* utils */
int	wordlen(char *str, int space_opt)
{
	char	*wordend;

	wordend = find_wordend(str, space_opt);
	// printf("wordend: %s\n", wordend);
	if (!wordend)
		return (0);
	if (!*wordend)
		return (wordend - str); //wordend가 널문자 일 때 그대로 반환
	return (wordend - str + 1);  //아닐 때는 널문자 자리 + 1 해주고 반환 
}

/* token lst */


/* pipe.c */
int	count_pipe(char *buffer)
{
	int	n;
	int len;

	n = 1; //buffer가 비어있어도 pipe는 무조건 1개 생성
	while (buffer && *buffer)
	{	 
		len = wordlen(buffer, PIPE); // |의 개수 
		// printf("len: %d\n", len);
		if (len == 0)
			len = ft_strlen(buffer);
		buffer += len;
	 	if (*buffer == PIPE)
			n++;
		// printf("buf: %s\n", buffer);
		// break;
		// buffer++;
	}
	return (n);
}

int handle_empty_pipe(char *buffer)
{
	int		pipe_open;

	pipe_open = 0;
	while (*buffer == SPACE)
		buffer++;
	if (*buffer == PIPE)
	{
		*(buffer + 1) = '\0';
		// printf("g_status: %ld", g_status);
		g_status = handle_error(NULL, buffer, SYN_TOK);
		return (g_status);
	}	
	while(*buffer)
	{
		if (*buffer == PIPE)
			pipe_open++;
		else if (pipe_open && *buffer != PIPE && !ft_isspace(*buffer))
			pipe_open--;
		if (pipe_open == 2)
		{
			*(buffer + 1) = '\0';
			g_status = handle_error(NULL, buffer, SYN_TOK);
			return (g_status);
		}
		buffer++;
	}
	return (EXIT_SUCCESS);
}

// int is(char *buffer)
// {
// 	while(ft_isspace(*buffer))
// 		buffer++;
// 	if (*buffer == PIPE)
// 	{
// 		*(buffer + 1) = '\0';
// 		g_status = handle_error(NULL, buffer, SYN_TOK);
// 		return (TRUE);
// 	}
// 	return (FALSE);
// }

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
		{
			return (NULL);
		}
		else if (ispipeopen(vbuffer) == TRUE) //열린 pipe 확인
		{
			vbuffer = get_pipeline(vbuffer); //추가입력 받기 
			if (!vbuffer) //입력 term 종료 했을 때, 메모리 부족할 때
			{
				free(headbuffer);
				g_status = handle_error(NULL, NULL, SYN_TERM);
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

/* handle empty */
int handle_empty_redirect(char *str)
{
	str = find_filename_start(str);
	if (ft_isresword(*str) == TRUE)
	{	
		*(str + 1) = '\0';
		g_status = handle_error(NULL, str, SYN_TOK);
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
		str = strtrim_free(str, " |");
		// free (old_str);
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
	// if (!*str)
	// 	return (str);
	while (*str)
	{
		len = wordlen(str, 0);
		if ((*str == SGL_QUOTE || *str == DBL_QUOTE) && len)
		{
			new_str = substr_free(str, 1 , len - 2);
			// new_str = ft_substr(str + 1, 0, len - 2);
			// free(old_str);
			if (!new_str)
				return (new_str);
			break;
		}
		else 
		{
			new_str = old_str;
			break ;
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

// char *replace_pcode(char *str)
// {
// 	char *envp; 

// 	envp = NULL;
// 	str++;
// 	if (*str== '?')
// 	{
// 		envp = ft_itoa(g_status);
// 		if (!envp)
// 			return (NULL);
// 		str++;
// 	}
// 	return (envp);
// }

// char *replace_val_env(t_deques *deqs, char *key)
// {
// 	char	*envp;

// 	envp = get_val_deq(deqs, key);
// 	if (!envp)
// 		return (NULL);
// 	return (envp);
// }



// char *get_key2(char *str)
// {
// 	char	*key; 
// 	int 	len;
// 	// int 	len2;

// 	len = strchrlen(str, SPACE);
// 	printf("getkeylen:%d\n", len);
// 	if (len == (int)ft_strlen(str))
// 		len = strchrlen(str, DOLLAR);
// 	key = ft_substr(str, 0, len);
// 	if (!key)
// 		return (NULL);
// 	return (key);
// }

// char *replace_dollar(t_deques *deqs, char *str, char *dst, int *len)
// {
// 	char	*envp;
// 	int		len;
// 	char 	*key;

// 	*len = 0;
// 	(str)++;
// 	// while (*str)
// 	// {
// 		// envp = replace_pcode(*str);
// 		// if (envp)
// 			// (*str)++;
// 		// else
// 		// {
// 	key = get_key2(*str);
// 	printf("key: %s\n", key);
// 	if (!key)
// 		return (NULL);
// 	envp = replace_val_env(deqs, key);
// 	len = ft_strlen(key) - 1;
// 	if (len >= 0)
// 		len = ft_strlen(*str);
// 	free(key);
// 	// str += len;
// 		// }
// 	dst = strjoin_free(dst, envp);
// 	printf("dst: %s\n", dst);
// 	printf("str: %s\n", *str);


// 	if (!dst)
// 		return (NULL);
// 		// (*str)++;
// 	// }
// 	return (dst);
// }


// char *replace_envp(t_deques *deqs, char *str)
// {
// 	char	*headstr;
// 	char	*dst;
// 	int		len;
	
// 	dst = ft_calloc(2, sizeof(char));
// 	if (!dst)
// 		return (NULL);
// 	headstr = str;
// 	// printf("re_str: %s\n", str);
// 	while (*str)
// 	{
// 		printf("dst: %s\n", dst);

// 		len = strchrlen(str, DOLLAR);
// 		if (len)
// 		{
// 			dst = strjoin_free(dst, ft_substr(str, 0, len));
// 			// dst = substrjoin(str, 0, len, dst);
// 			// printf("dst: %s\n", dst);
// 			str += len;
// 			if (!str)
// 				return (dst);
// 		}
// 		// printf("%s\n", str);
// 		// while(*str == DOLLAR)
// 			// str++;
// 		if (*str == DOLLAR) 
// 		{
// 			char *envp = replace_pcode(str);
// 			if (envp)
// 			{
// 				dst = strjoin_free(dst, envp);
// 				str++;
// 				printf("str:: %s\n", str);
// 			}
// 			// dst = replace_dollar(deqs, &str, dst, &len);
	

			
// 		}
// 		else
// 		{
// 			len = strchrlen(str, DOLLAR);
// 			dst = substrjoin(str, 0, len, dst);
// 			str += len;
// 		}

// 		if (!dst || !*str)
// 			break ;
// 		str++;
// 	}
// 	free(headstr);
// 	// printf("re_dst: %s\n", dst);
// 	return (dst);
// }



char *replace_pcode(char *str)
{
	char *envp;

	envp = NULL;
	str++;
	if (*str == '?')
	{
		envp = ft_itoa(g_status);
		if (!envp)
			return (NULL);
	}
	return (envp);
}


char *replace_val_env(t_deques *deqs, char *key)
{
	char	*envp;

	envp = get_val_deq(deqs, key);
	return (envp);
}

char *get_key2(char *str)
{
	char	*key; 
	int 	len;
	// int 	len2;

	len = strchrlen(str, SPACE);
	// printf("getkeylen:%d\n", len);
	if (len == (int)ft_strlen(str))
		len = strchrlen(str, DOLLAR);
	key = ft_substr(str, 0, len);
	if (!key)
		return (NULL);
	return (key);
}

// char *replace_dollar(t_deques *deqs, char *str, char *dst, int *len)
// {
// 	char	*envp;
// 	int		len;
// 	char 	*key;

// 	*len = 0;
// 	(str)++;
// 	// while (*str)
// 	// {
// 		// envp = replace_pcode(*str);
// 		// if (envp)
// 			// (*str)++;
// 		// else
// 		// {
// 	key = get_key2(*str);
// 	printf("key: %s\n", key);
// 	if (!key)
// 		return (NULL);
// 	envp = replace_val_env(deqs, key);
// 	len = ft_strlen(key) - 1;
// 	if (len >= 0)
// 		len = ft_strlen(*str);
// 	free(key);
// 	// str += len;
// 		// }
// 	dst = strjoin_free(dst, envp);
// 	printf("dst: %s\n", dst);
// 	printf("str: %s\n", *str);


// 	if (!dst)
// 		return (NULL);
// 		// (*str)++;
// 	// }
// 	return (dst);
// }

char *replace_dollar(t_deques *deqs, char *str, int *len)
{
	char *envp;
	char *key;
	
	envp = NULL;
	str++;
	key = get_key2(str);
	if (!key)
		return (NULL);
	envp = replace_val_env(deqs, key);
	*len = ft_strlen(key) + 1;
	// if (*len <= 0)
		// *len = ft_strlen(str);
	printf("keylen: %d\n", *len);
	printf("envp: %s\n", envp);
	free(key);
	return (envp);
}


char *replace_envp(t_deques *deqs, char *str)
{
	char	*envp;
	char	*headstr;
	char	*dst;
	int		len;
	(void) deqs;

	dst = ft_calloc(2, sizeof(char));
	if (!dst)
		return (NULL);
	headstr = str;
	while (*str)
	{
		//환경변수가 아닌 문자열
		len = strchrlen(str, DOLLAR);
		// printf(">>%s\n", str + len);
		if (len)
			dst = strjoin_free(dst, ft_substr(str, 0, len));
		else if (*str == DOLLAR) // $문자열
		{
			envp = replace_pcode(str);
			if (envp)
			{
				dst = strjoin_free(dst, envp);
				len = 2;
			}
			else //환경 변수 달려 
			{
				envp = replace_dollar(deqs, str, &len);
				if (envp)
				{
					dst = strjoin_free(dst, envp);
					break ;
				}
				break ;

			}
			if (!envp)
			{
				strjoin_free(dst, ft_substr(str, 0, 2));
				len = 2;
			}
		}
		break ;

		printf("dst: %s\n", dst);
		printf ("ok\n");		
		str += len;
		printf("str: %s\n", dst);
		// printf(">str: %s\n", dst);
	}
	free(headstr);
	
	return (dst);
	
	
}


char *replace_word(t_deques *deqs, char *str)
{
	char *new_str;

	// printf("start\n");
	new_str = trim_space(str);
	// printf("new_str1: %s#\n", new_str);
	if (ft_strchr(str, DOLLAR))
	{
		new_str = replace_envp(deqs, new_str);
		// printf("new_str2: %s#\n", new_str);
		if (*new_str)
			new_str = trim_quote(new_str);
	}
	else 
		new_str = trim_quote(new_str);
	if (!*new_str)
		return (NULL);
		// free(new_str);
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
			g_status = handle_error(NULL, "newline", SYN_TOK);
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
	int flag;

	n = 1;
	flag = 0;
	while (argv && *argv)
	{
		len = wordlen(argv, SPACE);
		if (*(argv + len) == SPACE || !*(argv + len))
			n++;		
		if (len)
			argv += len;
		else
			break ;
	}
	return (n);
}	
char *find_spacend(char *str);


int isemptyenvp(char *str, t_deques *envps)
{
	char *str2;
	char *envp;

	if (*str != '$')
		return (FALSE);
	str2 = ft_strdup(str);
	envp = replace_envp(envps, str2);
	if (!*envp)
		return (TRUE);
	return (FALSE);
}




char **get_argvs(char *word, char *argv, t_deques *envps)
{ 
	char	**argvs;
	char	*new; 
	int		n; 
	int		i;
	int		len;
	int		next;
	char	*start;

	start = argv;
	n = count_argv(argv);
	printf("n: %d\n", n);
	argvs = ft_calloc(n + 2, sizeof(char *));
	if (word)
		argvs[0] = ft_strdup(word);
	i = 1;
	len = 0;
	while (i < n && argv)
	{
		while (isspace(*argv))
		{
			argv++;
			len++;
		}
		// argv = find_spacend(argv);
		// if (*(argv + 1) == PIPE)
		// 	argv++;
		// printf("argv: %s\n", argv);
		next = wordlen(argv, SPACE);
		// printf("len: %d\n", len);
		if (!next)
			next = ft_strlen(argv);
		// printf("next: %d\n", next);
		
		len += next;
		if (*(argv + next) == SPACE || !*(argv + next))
		{
			// printf("len: %d\n", len);
			new = ft_substr(start, 0, len);
			if (isemptyenvp(new, envps) == TRUE)
			{
				argv += next;
				while (ft_isspace(*argv))
					argv++;
				continue;
			}
			argvs[i] = replace_word(envps, new);
			start += len;
			len = 0;
			if (argvs[i])
				i++;
		}
		// printf("argvs: %s\n", argvs[i]);
		// printf("argvs[%d]: %s\n", i - 1, argvs[i - 1]);
		argv += next;
		// if (!argvs[i] && argv)
			// argvs[i] = ft_substr("", 0, 1);
	}
	// int j = 0;
	// while (argvs[j])
	// {
	// 	printf("%d: %s#\n",j, argvs[j]);
	// 	j++;
	// }
	return (argvs);
}

/* tokenizer */
int token_redirect(t_token **token, t_deques *envps, char *str)
{
	int		typeno;
	char	*filename;
	int		filenamelen;
	// t_token *newtoken;

	while(*str)
	{
		str = find_redirect_start(str);	// printf("redirect start: %s\n", str);
		if (!*str)
			return (EXIT_SUCCESS);
		typeno = read_redirect_typeno(str);
		str = find_filename_start(str);
		filename = replace_filename(str, typeno);
		if (!filename)
			return (g_status);
		if (typeno >= T_DLESS && typeno <= T_GREAT)
		{
			filenamelen = wordlen(str, SPACE);
			filename = replace_word(envps, filename);
			add_tokenright(token, typeno, filename, NULL);
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

int add_tokenrightwords(t_token **token, char *word, char **argvs)
{
	if (!word)
	{
		word = ft_calloc(2, sizeof(char));
		if (!word)
			return (EXTRA_ERROR);
	}
	if (!argvs)
	{
		argvs = ft_calloc(2, sizeof(char*));
		if (!argvs)
			return (EXTRA_ERROR);
		*argvs = ft_calloc(2, sizeof(char));
		if (!*argvs)
			return (EXTRA_ERROR);
	}

	if (add_tokenright(token, T_CMD_WORD, word, argvs) == EXTRA_ERROR)
		return (EXTRA_ERROR);
	return (EXIT_SUCCESS);
}


char *get_word(t_deques *envps, char *words)
{
	char	*word;
	int		len;

	words = ft_ltrim(words);
	len = wordlen(words, SPACE);
	// printf("wordlen: %d\n", len);
	if (len == 0)
		len = ft_strlen(words);
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
	argv = strtrim_free(argv, " ");
	argvs = get_argvs(word, argv, envps);
	// int i = 0;
	// while (argvs[i])
	// 	printf(": %s\n",argvs[i++]);
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
		// if (word)
		argvs = get_argvs_center(envps, words, word);;
		free(headwords);
	}
	(void) token;

	// int i = 0;
	// while (argvs && argvs[i])
	// 	printf("a_cent: %s\n",argvs[i++]);
	if (add_tokenrightwords(token, word, argvs) == EXTRA_ERROR)
		return (EXTRA_ERROR);
	return (EXIT_SUCCESS);
}

void token_pipe(t_token **token)
{
	add_tokenright(token, T_PIPE, NULL, NULL);
}

t_token *tokenizer(t_deques *envps, char **strs)
{
	(void) envps;
	t_token *token;


	token = ft_calloc(1, sizeof(t_token));
	if (handle_empty_redirects(strs) == EXIT_FAILURE)
		return (NULL);
		// printf("Ok\n");
	while (strs && *strs)
	{	
		token_pipe(&token);
		// printf("Ok1\n");
		
		if (token_redirect(&token, envps, *strs) == SYNTAX_ERROR)
			return (NULL);
		// printf("Ok2\n");

		token_word(&token, envps, *strs);
		// printf("Ok3\n");

		strs++;
	}
	/*token 출력*/
	// debug_token(token);
	return (token);
}

/* lexer */
int	lexer(t_shell *shell, char *vbuffer)
{
	(void) shell;
	char	**vbuffers;

	vbuffers = split_pipe(vbuffer);
	free(vbuffer);
	if (!vbuffers)
		return(EXTRA_ERROR);
		// return (EXIT_FAILURE);
	// /*buffers 출력*/
	debug_buffers(vbuffers);

	// shell->t 
	// printf("%p\n", shell->t);
	// printf("===tokenizer start===\n");

	shell->t = tokenizer(shell->data.envps, vbuffers); //leaks
	// printf("%p\n", shell->t);

	free_strs(vbuffers);
	if (!shell->t)
		return (EXIT_FAILURE);
	// printf("===tokenizer complete===\n");
	
	return(EXIT_SUCCESS);
}

/* parslines.c */
int parselines(t_shell *shell, char *buffer)
{
	char *vbuffer;

	// g_status = 0;
	// printf("buffer: %s\n", buffer);
	vbuffer = get_valid_buffer(buffer);
	// printf("vbuffer: %s\n", vbuffer);
	// free (buffer);
	if (!vbuffer)
		return (EXIT_FAILURE);
	vbuffer = strtrim_free(vbuffer, " ");
	if (!vbuffer)
		return (EXIT_FAILURE);
	
	// if (!*vbuffer)
	// 	return (EXIT_FAILURE);

	// lexer(shell, vbuffer);
	// printf("===lexer start===\n");
	if (lexer(shell, vbuffer) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	// printf("===lexer complete===\n");
	parser(&shell->t);
	return (EXIT_SUCCESS);
}
