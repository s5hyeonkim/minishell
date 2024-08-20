#include "../minishell.h"

/* utils */
char *find_notspace(char *str)
{
	while(str && *str && ft_isspace(*str))
		str++;
	return (str);
}

/* quote_utils */
char *find_spacend(char *str)
{
	while(str && *str && ft_isspace(*str) && ft_isspace(*(str + 1)))
		str++;
	return (str);
}

int issgldbl(char chr)
{
	if (chr == SGL_QUOTE)
		return(T_SINGLE_QUOTES);
	else if (chr == DBL_QUOTE) 
		return(T_DOUBLE_QUOTES);
	return (0);
}

/* quote */
char *find_quotend(char *str, int flag)
{
	int sgl_open;
	int dbl_open;

	sgl_open = 0;
	dbl_open = 0;
	while(*str)
	{
		if (*str == SGL_QUOTE && !sgl_open && dbl_open && dbl_open--)
			return(ft_strchr(str, DBL_QUOTE) + 1);
		else if (*str == SGL_QUOTE && !sgl_open && !dbl_open)
			sgl_open = TRUE;
		else if (*str == DBL_QUOTE && !dbl_open && sgl_open && sgl_open--)
			return(ft_strchr(str, SGL_QUOTE));
		else if (*str == DBL_QUOTE && !dbl_open && !sgl_open)
			dbl_open = TRUE;
		else if	((*str == SGL_QUOTE && sgl_open--) || (*str == DBL_QUOTE  && dbl_open-- ))
			break;
		str++;
	}
	if ((flag == T_SINGLE_QUOTES && sgl_open) || (flag == T_DOUBLE_QUOTES && dbl_open))
		return (NULL);
	return (str + 1);
}


int ft_iswordhead(char chr)
{
	if (ft_isspace(chr) == TRUE \
	|| ft_ispipe(chr) == TRUE \
	|| ft_isquote(chr) == TRUE \
	|| ft_isredirect(chr) == TRUE)
		return (TRUE);
	return (FALSE);
}

char *find_wordend(char *str) //word: quote, space(not_quote), pipe, direct 기준
{
	str = find_notspace(str);
	if (*str == SGL_QUOTE || *str == DBL_QUOTE) //quote문 처리
		str = find_quotend(str, issgldbl(*str));
	else // quote, space(not_quote), pipe가 오면 반환 
		while (str && *str && ft_iswordhead(*str) == FALSE)
			str++;
	// printf("find_wordend:%s\n", str);
	return (str);
}

// char *find_wordend(char *str, i
// int main(int argc, char **argv)
// {
// 	char *buffer;
// 	char *next;

// 	buffer = readline("test>");
// 	while (*buffer)
// 	{
// 		printf("buffer: %s\n", buffer);
// 		next = find_wordend(buffer, SPACE, 0);
// 		if (!next)
// 			printf ("next is emtpy\n");
// 		if (next)
// 			buffer = next + 1;
// 	}
// 	printf("buffer: %s\n", buffer);

// }