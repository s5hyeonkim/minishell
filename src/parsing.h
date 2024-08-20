#ifndef PARSING_H
# define PARSING_H

# define PIPE '|'
# define SGL_QUOTE '\''
# define DBL_QUOTE '\"'
# define LESS '>'
# define GREAT '<'
# define DOLLAR '$'
# define SIGTERM_NULL 0

#include "../execute/deque/deque.h"

typedef enum e_typeno
{
	T_NULL,
	T_PIPE,
	T_SIMPLE_CMD,
	T_CMD_WORD, //cmd, filename, hereend 
	T_IO_REDIRECTS, //redirects
	T_DLESS,		// >>
	T_DGREAT,		// <<
	T_LESS,			// >
	T_GREAT, 		// < 
	T_DOUBLE_QUOTES,
	T_SINGLE_QUOTES
}	t_typeno;

typedef struct s_token
{
	t_typeno		type;
	char			*word; //cmd, filename, hereend 
	char			**argvs;
	struct s_token	*left;
	struct s_token	*right;
}	t_token;

/* buffer.c */ 
int	get_validbuffer(char *buffer, char **validbuffer);


/*----complete-----*/

/* sublibft.c */
int	strchrlen(char *str, int chr);
char	*substr_free(char *str, unsigned int start, size_t len);
char	*strjoin_free(char *str1, char *str2);
// char	*substrjoin (char *str, int start, int len, char *dst);
char	*substrjoin(char *dst, char *str, int len);

/* tlst.c */
int		set_token(t_token **t);
t_token	*tlst_lastright(t_token *t);
void	tlst_addright(t_token **t, t_token *newtoken);
void	tlst_addleft(t_token **t, t_token *newtoken);

/* resword.c */
int ft_isresword(char chr);
int ft_isredirect(char chr);
int ft_ispipe(char chr);
int ft_isquote(char chr);

/* handle_error.c */
int handle_empty_pipe(char *buffer);



/* pipe.c */
int ft_ispipeopen(char *buffer);
int handle_empty_pipe(char *buffer);
char **split_pipe(char *buffer);

/* redirect.c */
char	*find_filename_head(char *redirect_head);
char	*find_redirect_head(char *buffer);
int		read_redirect_typeno(char *str);
int		handle_empty_redirect(char *buffer);

/* wordlen.c */
int		wordlen(char *str);
char	*wordlen_pipe(char *startstr, int *len);
char	*wordlen_redirect(char *startstr, int *len);
char	*wordlen_filename(char *redirect_head, int *len);
char	*wordlen_word(char *startstr, int *len);

/* value.c */
char	*replace_value(t_deques *envps, char *str);

/* value_utils */
char	*get_novalue(char *str, int *len);
char	*get_dollar(char *str, int *len);
char	*get_status(char *str, int *len);
char	*get_env(t_deques *envps, char *keyhead, int *len);

// char	*get_env(t_deques *envps, char *keyhead, int *len);

/* utils.c */
char	*find_notspace(char *str);

/* find_wordend.c */
char *find_wordend(char *str);

int ft_isquotend(char *str, int flag);

/* argv.c */
char *get_words(char *str);
char **get_argvs(t_deques *envps, char *words);

/* parsing.c */
int	handle_error_withstr(char *exec, char *str, int len, int code);

/* token.c */
int	tokenizer(t_token **token, t_deques *envps, char **strs);

/* token_utils.c */
int	add_tokenright_words(t_token **token, char *word, char **argvs);
int	add_tokenright(t_token **token, int type, char *word, char **argvs);

t_token	*parser(t_token **token);

int	set_next_cmd(char **s);

/* debug.c */
void debug_buffers(char **buffers);
void debug_token(t_token *token);
void debug_tree(t_token *dsttoken, int leftright, int i);

/* parsing.c */
char *replace_word(t_deques *envps, char *str);

#endif