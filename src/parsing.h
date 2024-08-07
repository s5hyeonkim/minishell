#ifndef PARSING_H
# define PARSING_H

# define PIPE '|'
# define SGL_QUOTE '\''
# define DBL_QUOTE '\"'
# define LESS '>'
# define GREAT '<'
# define DOLLAR '$'

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
	char			*argv;
	struct s_token	*left;
	struct s_token	*right;
}	t_token;

// char *next_word(char *str, int space_opt);
char *next_word(char *str, int space_opt, int ltrim_opt);

// char *next_word(char *str);
// char *next_quoteline(char *str);
char *ft_ltrim(char *str);
char *next_quote(char *str, int flag);
int is_sgldbl(char chr);
t_token	*parser(t_token **token);


t_token *tlst_lastright(t_token *t);
t_token *tlst_lastleft(t_token *t);
void tlst_addright(t_token **t, t_token *newtoken);
void tlst_addleft(t_token **t, t_token *newtoken);

int set_token(t_token **t);


void print_tree(t_token *dsttoken, int leftright, int i);

#endif