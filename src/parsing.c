#include "../minishell.h"

// /* token lst */

void add_token(t_shell *shell, int type, char *word, char **argv)
{
	t_token *newtoken;

	if (set_token(&newtoken))
		exit_process(shell, NULL, EXTRA_ERROR);
	newtoken->type = type;
	newtoken->word = word;
	newtoken->argv = argv;
	tlst_addright(&shell->t, newtoken);
}

int count_pipe(char *buffer)
{
	int	n;

	n = 1;
	while (buffer && *buffer)
	{	
		if (*buffer == SGL_QUOTE || *buffer == DBL_QUOTE)
		{
			if (*(buffer + 1))
			{	
				buffer = find_wordend(buffer, 0, 1) + 1 - 1;
				// printf("buffer: %s\n", buffer);
				if (!buffer)
					break ;
			}
		}
		else if (*buffer == PIPE)
			n++;
		buffer++;
	}
	return (n);
}

char *next_pipe(char *buffer)
{
	while (buffer && *buffer)
	{	
		if (*buffer == SGL_QUOTE || *buffer == DBL_QUOTE)
		{
			buffer = find_wordend(buffer, 0, 1) + 1 - 1;
			if (!buffer)
				return (buffer + 1);
			// continue;
		}
		else if (*buffer == PIPE)
			return (buffer + 1);
		buffer++;
	}
	return (NULL);
}

// int is_emptypipe(char *str)
// {
// 	while (ft_isspace(*str) || *str == PIPE)
// 		str++;
// 	if (*str == '\0')
// 		return(TRUE);
// 	return(FALSE);
// }


int handle_empty_pipe(char *buffer)
{
	char	*head_buffer;
	int		pipe_open;

	head_buffer = buffer;
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
			free(head_buffer);
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


char *get_pipeline(char *dst_buffer)
{
	char *old_buffer;
	char *src_buffer;

	old_buffer = dst_buffer;
	src_buffer = readline(">");
	if (!src_buffer)
		dst_buffer = NULL;
	else
	{
		dst_buffer = ft_strjoin(dst_buffer, src_buffer);
		free(src_buffer);
	}
	free(old_buffer);
	return (dst_buffer);
}

char *get_valid_buffer(char *buffer)
{
	while(1)
	{
		if (handle_empty_pipe(buffer) == SYNTAX_ERROR) //비어있는 pipe 에러처리
			return (NULL);
		else if (ispipeopen(buffer) == TRUE) //열린 pipe 확인
		{
			buffer = get_pipeline(buffer); //추가입력 받기 
			if (buffer == NULL) //입력 term 종료 했을 때
			{
				status = handle_error(NULL, NULL, SYN_TERM);
				return (NULL);
			}
		}
		else
			break;
	}
	return (buffer);
}


char **split_pipe(char *buffer)
{
	char	**strs;
	int		pipe_num;
	int		index;
	int		strlen;

	if (!buffer)
		return (NULL);
	pipe_num = count_pipe(buffer);
	strs = ft_calloc(pipe_num + 1, sizeof(char *));
	index = 0;
	while (index < pipe_num)
	{
		while (ft_isspace(*buffer))
			buffer++;
		strlen = next_pipe(buffer) - buffer;
		if (strlen)
			strs[index++] = ft_substr(buffer, 0, strlen);
		buffer += strlen;
	}
	return (strs);
}

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

//redirect의 typeno을 읽어주는 함수 
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


char *get_token_trim(char *str)
{
	char	*old_str;
	
	if (ft_strchr(str, SPACE) || ft_strchr(str, PIPE))
	{
		old_str = str;
		str = ft_strtrim(str, " |");
		free (old_str);
	}
	return (str);
}

char	*substr_strjoin (char *str, char *dst, int start, int len)
{
	char	*head;
	char	*s2;

	head = dst;
	s2 = ft_substr(str, start, len);
	dst = ft_strjoin(dst, s2);
	free(s2);
	free(head);
	return (dst);
}

char *trim_quote(char *str)
{
	char	*old_str;
	char 	*new_str;
	int		len;

	old_str = str;
	new_str = NULL;
	while (str && *str)
	{
		char *next = find_wordend(str, 0, 0) + 1;
		// printf("next: %s\n", next);
		if (next)
		{
			len = next - str;
			// printf("len2: %d\n", len);
			if ((*str == SGL_QUOTE || *str == DBL_QUOTE))
				new_str = substr_strjoin(str + 1, new_str, 0, len - 2);
			else 
				new_str = substr_strjoin(str, new_str, 0, len--);
		}
		else 
		{
			len = ft_strlen(str);
			// printf("len3: %d\n", len);
			new_str = substr_strjoin(str, new_str, 0, len);
		}
		str += len;
	}
	free(old_str);
	return (new_str);
}

char *replace_envp2(t_deques *deqs, char **str, char *dst)
{
	char	*old_dst;
	char	*envp;
	char	*key; 

	if (**str && **str == DOLLAR)
	{
		(*str)++;
		if (**str && **str == '?')
		{
			envp = ft_itoa(status);
			(*str)++;
		}
		else
		{
			key = ft_substr(*str, 0, ft_strchr(*str, ' ') - *str);
			envp = get_val_deq(deqs, key);
			(*str) += (ft_strlen(key) - 1);
			free(key);
		}
		old_dst = dst;
		dst = ft_strjoin(dst, envp);
		free(old_dst);
		free(envp);
		return (dst);
	}
	return (dst);
}

char *replace_envp(t_deques *deqs, char *str)
{
	char	*head;
	char	*dst;
	int		len;
	
	dst = ft_calloc(1, sizeof(char));
	if (!ft_strchr(str, DOLLAR))
		return (str);
	head = str;
	while (str && *str)
	{
		len = 0;
		if (*str && *str == DOLLAR)
			dst = replace_envp2(deqs, &str, dst);
		else
		{
			if (ft_strchr(str, DOLLAR))
			{
				len = ft_strchr(str, DOLLAR) - str - 1;
				dst = substr_strjoin(str, dst, 0, len + 1);
			}
			else
			{
				len = ft_strlen(str);
				dst = substr_strjoin(str, dst, 0, len);
			}
			str += len;
		}
		str++;
	}
	free(head);
	return (dst);
}


char *replace_word(t_deques *deqs, char *str)
{
	(void) deqs;
	(void) str;
	char *new_str;

	new_str = get_token_trim(str);
	// printf("new_str: %s\n", new_str);
	new_str = replace_envp(deqs, new_str);
	// printf("new_str2: %s\n", new_str);
	new_str = trim_quote(new_str);
	// printf("new_str3: %s\n", new_str);
	return (new_str);
}

char *get_filename(char *str, int typeno)
{
	char	*filename;
	int		filenamelen;

	filename = NULL;
	if (typeno >= T_DLESS && typeno <= T_GREAT)
	{	
		filenamelen = find_wordend(str, SPACE, 1) - str + 1;
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

int wordlen(char *str, int space_opt)
{
	return ( find_wordend(str, space_opt, 1) - str + 1);
}




char *get_words(char *dst_words, char *str, int len)
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


int	count_argv(char *argv)
{
	int n;
	size_t	size;
	size_t	index;

	size = ft_strlen(argv);
	n = 0;
	index = 0;
	// printf(">%s#\n", argv);
	while (argv && *argv)
	{
		n++;
		argv = find_wordend(argv, SPACE, 0);
		if (argv)
			argv++;
		// printf(">>%s\n", argv);
	}
	return (n);
}


char **get_argvs(char *word, char *argv)
{
	char	**argvs;
	char 	*next;
	int		n; 
	int		i;

	n = count_argv(argv) + 1;
	// printf("%d\n", n);
	argvs = ft_calloc(n, sizeof(char *));
	argvs[0] = ft_strdup(word);
	// argvs[1] = 0;
	i = 1;
	while (i < n && argv)
	{
		// printf("argv: %s\n", argv);
		next = find_wordend(argv, SPACE, 0) + 1;
		// printf("next-argv: %ld\n", next - argv);
		argvs[i] = ft_substr(argv, 0, next - argv);
		argv = next;
		// printf("argvs: %s\n", argvs[i]);
		i++;
	}
	argvs[i] = 0;
	return (argvs);
}

int token_redirect(t_shell *shell, char *str)
{
	int		typeno;
	char	*filename;
	int		filenamelen;

	while(*str)
	{
		str = find_redirect_start(str);	// printf("redirect start: %s\n", str);
		typeno = read_redirect_typeno(str);
		str = find_filename_start(str);
		filename = get_filename(str, typeno);
		if (!filename)
			return (status);
		if (typeno >= T_DLESS && typeno <= T_GREAT)
		{
			filenamelen = wordlen(str, SPACE);
			filename = replace_word(shell->data.envps, filename);
			add_token(shell, typeno, filename, NULL);
			str += filenamelen;
		}
		else
			str++;
	}
	return (EXIT_SUCCESS);
}

void token_word(t_shell *shell, char *str)
{
	char	*words;
	char	*word;
	char	*argv;
	int		len;
	(void) shell;
	(void) argv;
	(void) word;

	argv = NULL;
	words = NULL;
	while (*str)
	{
		len = find_redirect_start(str) - str;
		if (len > 0)
			words = get_words(words, str, len);
		str += len;
		// printf("get_words len:%d\n", len);
		if (*str)
		{
			str = find_filename_start(str);
			len = find_wordend(str, SPACE, 0) + 1 - str;
			// printf("get_word len2: %d\n", len);
			str += len;
		}
	}
	if (words)
	{
		len = 0;
		// printf("words: %s\n", words);
		// printf("len: %d\n", len);
		// printf ("next: %p\n",find_wordend(words, SPACE, 1));
		char *next = find_wordend(words, SPACE, 1) + 1;
		if (next)
			len = next - words;
		// printf("len: %d\n", len);
		// printf("next: %s\n ", next);
		// if (len <= 0)
		// 	len = ft_strlen(words);
		// printf("len: %d\n", len);
		word = ft_substr(words, 0, len);
		argv = ft_substr(words + len, 0, ft_strlen(words + len));
		// printf("words + len = %s\n", argv);
		// printf("word: %s\n", word);
		word = replace_word(shell->data.envps, word);
		argv = replace_word(shell->data.envps, argv);
		// printf("argv: %s\n", argv);
		char **argvs = get_argvs(word, argv);
		// free(words);
		add_token(shell, T_CMD_WORD, word, argvs);
	}
}

void token_pipe(t_shell *shell)
{
	add_token(shell, T_PIPE, NULL, NULL);
}

int tokenizer(t_shell *shell, char **strs)
{
	//예외처리
	if (handle_empty_redirects(strs) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	//token
	while (strs && *strs)
	{	
		token_pipe(shell);
		if (token_redirect(shell, *strs) == SYNTAX_ERROR)
			return (status);
		token_word(shell, *strs);
		strs++;
	}
	/*토큰 출력*/
	// debug_token(shell->t);
	return (0);
}

void	lexer(t_shell *shell, char *buffer)
{
	char	**buffers;

	buffers = split_pipe(buffer);
	if (!buffers)
		return ;
	free(buffer);
	// debug_buffers(buffers);
	tokenizer(shell, buffers);
	free_strs(buffers);
}

void	parselines(t_shell *shell, char *buffer)
{
	buffer = get_valid_buffer(buffer);
	if (!buffer)
		return ;
	lexer(shell, buffer);
	parser(&shell->t);
}

