/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 17:08:15 by yubshin           #+#    #+#             */
/*   Updated: 2024/08/26 13:14:07 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# define PIPE '|'
# define SGL_QUOTE '\''
# define DBL_QUOTE '\"'
# define LESS '>'
# define GREAT '<'
# define DOLLAR '$'
# define SPACE ' '

# include "../execute/deque/deque.h"

volatile long	g_status;

typedef enum e_typeno
{
	T_NULL,
	T_PIPE,
	T_SIMPLE_CMD,
	T_CMD_WORD,
	T_IO_REDIRECTS,
	T_DLESS,
	T_DGREAT,
	T_LESS,
	T_GREAT,
	T_DOUBLE_QUOTES,
	T_SINGLE_QUOTES
}	t_typeno;

typedef struct s_token
{
	t_typeno		type;
	char			*word;
	char			**argvs;
	struct s_token	*left;
	struct s_token	*right;
}	t_token;

/* get_buffer.c */
int		get_validbuffer(char *buffer, char **validbuffer);

/* next_cmd.c */
int		set_next_cmd(char **s);

/* pipe.c */
int		ft_ispipeopen(char *buffer);
int		handle_empty_pipe(char *buffer);
char	**split_pipe(char *buffer);

/* redirect.c */
char	*find_filename_head(char *redirect_head);
char	*find_redirect_head(char *buffer);
int		read_redirect_typeno(char *str);
int		handle_empty_redirect(char *buffer);

/* argv.c */
int		get_words(char **words, char *str);
int		get_word(t_deques *envps, char **word, char *words);
int		get_argvs(t_deques *envps, char ***argvs, char *words);

/* wordend.c */
char	*find_wordend(char *str);
char	*is_closed_quotation(char *str);

/* wordlen.c */
int		wordlen(char *str);
char	*wordlen_pipe(char *startstr, int *len);
char	*wordlen_redirect(char *startstr, int *len);
char	*wordlen_filename(char *redirect_head, int *len);
char	*wordlen_word(char *startstr, int *len);

/* tokenizer.c */
int		tokenizer(t_token **token, t_deques *envps, char **strs);

/* token_utils.c */
int		set_token(t_token **t);
int		add_tokenright(t_token **token, int type, char *word, char **argvs);

/* token_lst.c */
t_token	*tlst_lastright(t_token *t);
void	tlst_addright(t_token **t, t_token *newtoken);
void	tlst_addleft(t_token **t, t_token *newtoken);

/* parser.c */
int		parser(t_token **token);

/* replace_word.c */
char	*replace_word(t_deques *envps, char *str);

/* replace_value.c */
char	*replace_value(t_deques *envps, char *str);

/* replace_value_utils */
char	*get_dollar(char *str, int *len);
char	*get_status(int *len);
char	*get_env(t_deques *envps, char *keyhead, int *len);

/* replace_quote.c */
char	*replace_quote(char *str);

/* resword.c */
int		ft_isresword(char chr);
int		ft_isredirect(char chr);
int		ft_ispipe(char chr);
int		ft_isquote(char chr);

/* sublibft.c */
int		strchrlen(char *str, int chr);
char	*strjoin_free(char *str1, char *str2);
char	*substrjoin_free(char *dst, char *str, int len);
char	*strchr_end(char *str, int chr);

/* utils.c */
char	*find_notspace(char *str);
int		handle_error_withstr(char *exec, char *str, int len, int code);

/* debug.c */
void	debug_buffers(char **buffers);
void	debug_token(t_token *token);
void	debug_tree(t_token *dsttoken, int leftright, int i);

char	*replace_quote(char *str);

#endif