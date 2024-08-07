#include "../minishell.h"

// /* token lst */
t_token *tlst_lastright(t_token *t)
{
	if (t == NULL)
		return (NULL);
	while (t->right != NULL)
		t = t->right;
	return (t);
}

t_token *tlst_lastleft(t_token *t)
{
	if (t == NULL)
		return (NULL);
	while (t->left != NULL)
		t = t->left;
	return (t);
}

void tlst_addright(t_token **t, t_token *newtoken)
{
	t_token *lasttoken;

	if (*t == NULL)
		*t = newtoken;
	else if (newtoken != NULL)
	{
		lasttoken= tlst_lastright(*t);
		lasttoken->right = newtoken; 
		// if (lasttoken)
		// 	newtoken->left = lasttoken;
		// else 
		// 	newtoken->left = NULL;
		// printf("left: %p\n", newtoken->left);
		// printf("now: %p\n", newtoken);
		// printf("right: %p\n", newtoken->right);
	}
}

void tlst_addleft(t_token **t, t_token *newtoken)
{
	t_token *lasttoken;

	if (*t == NULL)
		*t = newtoken;
	else if (newtoken != NULL)
	{
		lasttoken= tlst_lastleft(*t);
		lasttoken->left = newtoken; 
		// newtoken->right = lasttoken;
	}
}

void add_token(t_shell *shell, int type, char *word, char *argv)
{
	t_token *newtoken;

	if (set_token(&newtoken))
		exit_process(shell, NULL, EXTRA_ERROR);
	newtoken->type = type;
	newtoken->word = word;
	newtoken->argv = argv;
	tlst_addright(&shell->t, newtoken);
}

/* syntax error */
/*
pipe - pipe밖에 없는 열이면 syntax err
	 - pipe로 끝나면 추가 입력 받음 
redirect - redirect 다음에 redirect/pipe를 제외한 filename 없으면 syntax err
	ㄴ rediect/pipe
	ㄴ newline
*/

//lexlst의 마지막의 직전 리스트의 조건을 확인한 후 syntax error 여부 확인
// int is_syntax_error(t_token *lexlst)
// {
// 	t_token *last_lexlst;

// 	last_lexlst = tlst_lastright(lexlst);
// 	if (is_control_operator(lexlst->left->type))
// 		return (SYNTAX_ERROR); //exit로 바꾸기 
// 	if (is_redirect_operator(lexlst->left->type))
// 		return (SYNTAX_ERROR);
// 	return (0);
// }

int count_pipe(char *buffer)
{
	int n;

	n = 1;
	while (buffer && *buffer)
	{	
		if (*buffer == SGL_QUOTE || *buffer == DBL_QUOTE)
		{
			if (*(buffer + 1))
			{	
				buffer = next_word(buffer, 0, 1) + 1 - 1;
				// printf("buffer: %s\n", buffer);
				if (!buffer)
				{
					// printf("ok1\n");
					break ;
				}
				// printf("ok2\n");
			}
		}
		else if (*buffer == PIPE)
			n++;
		buffer++;
	}
	// printf("ok3\n");
	// printf("%d\n", n);
	return (n);
}

char *next_pipe(char *buffer)
{
	while (buffer && *buffer)
	{	
		if (*buffer == SGL_QUOTE || *buffer == DBL_QUOTE)
		{
			buffer = next_word(buffer, 0, 1) + 1 - 1;
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


int ft_isreserved(char chr)
{
	if (chr == LESS || chr == GREAT || chr == PIPE)
		return (TRUE);
	return (FALSE);
}

char *next_redirect(char *str)
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

int read_redirect_typeno(char *str)
{
	while (ft_isspace(*str))
		str++;
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
	str = next_redirect(str);
	if (ft_isreserved(*str) == TRUE)
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
	
	if (ft_strchr(str, ' ') || ft_strchr(str, '|'))
	{
		old_str = str;
		str = ft_strtrim(str, " |");
		free(old_str);
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
		char *next = next_word(str, 0, 0);
		// printf("next: %s\n", next);
		if (next)
		{
			len = next + 1 - str;
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

	// new_str = str;
	new_str = get_token_trim(str);
	// printf("new_str: %s\n", new_str);
	new_str = replace_envp(deqs, new_str);
	// printf("new_str2: %s\n", new_str);
	new_str = trim_quote(new_str);
	// printf("new_str3: %s\n", new_str);
	return (new_str);
}


int token_redirect(t_shell *shell, char *str)
{
	int		typeno;
	char	*filename;
	int		filename_len;

	while(*str)
	{
		while(*str == '\0' && *str != LESS && *str!= GREAT)
			str++;
		typeno = read_redirect_typeno(str);
		str = next_redirect(str);
		if (typeno >= T_DLESS && typeno <= T_GREAT)
		{	
			filename = ft_substr(str, 0, next_word(str, SPACE, 1) - str + 1);
			if (filename == NULL || *filename == '\0')
			{
				status = handle_error(NULL, "newline", SYN_TOK);

				return (status);
			}
		}
		if (typeno >= T_DLESS && typeno <= T_GREAT)
		{
			filename_len = next_word(str, SPACE, 1) - str + 1;
			filename = replace_word(shell->data.envps, filename);
			add_token(shell, typeno, filename, NULL);
			str += filename_len;
		}
		else
			str++;
		// printf("filename_lne: %d\n", filename_len);
	}
	// printf("ok10\n");
	return (EXIT_SUCCESS);
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


void token_word(t_shell *shell, char *str)
{
	char	*words;
	char	*word;
	char	*argv;
	int		len;
	(void) shell;
	(void) argv;

	argv = NULL;
	words = NULL;
	while (*str)
	{
		len = 0;
		while(str[len] && str[len] != LESS && str[len] != GREAT)
			len++;
		words = get_words(words, str, len);
		str += len;
		// printf("get_words len:%d\n", len);
		if (*str)
		{
			str = next_redirect(str);
			len = next_word(str, SPACE, 1) + 1 - str;
			// printf("get_word len2: %d\n", len);
			str += (len - 1);
		}
	}
	if (words)
	{
		len = 0;

		// printf("words: %s\n", words);
		// printf("len: %d\n", len);
		// printf ("next: %p\n",next_word(words, SPACE, 1));
		char *next = next_word(words, SPACE, 1);
		if (next)
			len = next + 1 - words;
		// printf("len: %d\n", len);
		
		// printf("next: %s\n ", next_word(words, SPACE, 1));
		if (len <= 0)
			len = ft_strlen(words);
		// printf("len: %d\n", len);
		word = ft_substr(words, 0, len);
		argv = ft_strjoin(ft_strdup(word), ft_substr(words + len, 0, ft_strlen(words + len)));
		// printf("word: %s\n", word);
		// printf("argv: %s\n", argv);
		free(words);
		word = replace_word(shell->data.envps, word);
		argv = replace_word(shell->data.envps, argv);
		add_token(shell, T_CMD_WORD, word, argv);
	}
}

void token_pipe(t_shell *shell)
{
	add_token(shell, T_PIPE, NULL, NULL);
}


void	tokentrim(t_token *token)
{
	char *old_word;
	char *old_argv;

	while (token)
	{
		if (token->word)
		{
			old_word = token->word;
			token->word = ft_strtrim(token->word, " ");
			free(old_word);
		}
		if (token->argv)
		{
			old_argv = token->argv;
			token->argv = ft_strtrim(token->argv, " ");
			free(old_argv);
		}
		token = token->right;
	}
}


int tokenizer(t_shell *shell, char **strs)
{
	//예외처리
	if (handle_empty_redirects(strs) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	
	//redirect 와 filename 담아서 lexlst에 넣기
	while (strs && *strs)
	{	
		token_pipe(shell);
		if (token_redirect(shell, *strs) == SYNTAX_ERROR)
			return (status);
		token_word(shell, *strs);
		strs++;
	}
	/*토큰 출력*/
	// t_token *token;
	// token = shell->t;
	// printf("---- token ----\n");
	// while (token)
	// {
	// 	printf("type: %d#\n", token->type);
	// 	printf("word: %s#\n", token->word);
	// 	printf("argv: %s#\n\n", token->argv);
	// 	token = token->right;
	// }
	// printf("-------------\n");
	return (0);
}

// && & || () -> WORD 처리  
//deque에 utils에서 환경변수 치환 하면 됨
//$? 따로 만들어줘야함  -> WORD로 저장 
//파이프와 파이프 사이에 명령어가 있는지 
void	lexer(t_shell *shell, char *buffer)
{
	(void) shell;
	char	**buffers;
	(void) buffers;
	(void) buffer;

	//파이프 기준으로 2차원 배열 만들기 
	buffers = split_pipe(buffer);
	if (!buffers)
		return ;

	free(buffer);
	// /*strs 출력*/
	// int i = 0;
	// while (buffers[i])
	// 	printf("%s#\n", buffers[i++]);
	//tokenizer
	tokenizer(shell, buffers);
	free_strs(buffers);
}


void	parselines(t_shell *shell, char *buffer)
{

	(void) shell;
	(void) buffer;
	//valid buffer 
	buffer = get_valid_buffer(buffer);
	if (!buffer)
		return ;
	// lexer : tokenizer 
	lexer(shell, buffer);
	//구문분석 : 트리 만들기 
	parser(&shell->t);
	// printf("token end\n");

}

