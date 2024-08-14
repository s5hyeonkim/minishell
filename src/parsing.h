#ifndef PARSING_H
# define PARSING_H

# define PIPE '|'
# define SGL_QUOTE '\''
# define DBL_QUOTE '\"'
# define LESS '>'
# define GREAT '<'
# define DOLLAR '$'
# define SIGTERM_NULL 0

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

/* sublibft.c */
int		ismadeofchr(char *str, char chr);
char	*substr_free(char *str, unsigned int start, size_t len);
char	*strjoin_free(char *str1, char *str2);
char	*substrjoin (char *str, int start, int len, char *dst);
char	*strtrim_free(char *str1, char *set);

/* tlst.c */
t_token	*tlst_lastright(t_token *t);
void	tlst_addright(t_token **t, t_token *newtoken);
void	tlst_addleft(t_token **t, t_token *newtoken);

/* token.c */
int set_token(t_token **t);
// int	add_tokenright(t_token *token, int type, char *word, char **argvs);
int	add_tokenright(t_token **token, int type, char *word, char **argvs);

/* resword.c */
int ft_isresword(char chr);
int ft_isredirect(char chr);

/* find.c */
char    *find_redirect_start(char *str);
char    *find_filename_start(char *str);
char	*find_pipeend(char *buffer);

/* QUOTE.C */
char *ft_ltrim(char *str);
int issgldbl(char chr);
char *find_quotend(char *str, int flag);
char *find_wordend(char *str, int spacepipe_opt);


t_token	*parser(t_token **token);

int	set_next_cmd(char **s);


void print_tree(t_token *dsttoken, int leftright, int i);

/*DEBUG*/
void debug_buffers(char **buffers);
void debug_token(t_token *token);
void debug_tree(t_token *dsttoken, int leftright, int i);

#endif