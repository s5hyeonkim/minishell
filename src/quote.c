#include "../minishell.h"

char *ft_ltrim(char *str)
{
	while(str && *str && ft_isspace(*str))
		str++;
	return (str);
}

int is_sgldbl(char chr)
{
	if (chr == SGL_QUOTE)
		return(T_SINGLE_QUOTES);
	else if (chr == DBL_QUOTE) 
		return(T_DOUBLE_QUOTES);
	return (0);
}

 //마지막 쿼트 반환 
char *next_quote(char *str, int flag)
{
	int sgl_open;
	int dbl_open;

	sgl_open = 0;
	dbl_open = 0;
	while(*str)
	{
		if (*str == SGL_QUOTE && !sgl_open && dbl_open && dbl_open--)
			return(ft_strchr(str, DBL_QUOTE));
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
	return (str );
}

// char *next_quoteline(char *str)
// {
// 	str = ft_ltrim(str);
// 	if (*str == SGL_QUOTE || *str == DBL_QUOTE)
// 	{
// 		str = next_quote(str, is_sgldbl(*str));
// 		return (ft_ltrim(str));
// 	}
// 	while (*str)
// 	{
// 		if (*str == SGL_QUOTE || *str == DBL_QUOTE)
// 			break;
// 		str++;
// 	}
// 	return (ft_ltrim(str));
// }

char *next_word(char *str, int space_opt, int ltrim_opt)
{
	(void) ltrim_opt;
	// if (ltrim_opt)
		// str = ft_ltrim(str);
	if (*str == SGL_QUOTE || *str == DBL_QUOTE)
	{
		str = next_quote(str, is_sgldbl(*str));
		// if (ltrim_opt)
			// return (ft_ltrim(str));
		return (str);
	}
	while (*str)
	{
		if (*str == SGL_QUOTE || *str == DBL_QUOTE)
			break ;
		else if (space_opt == SPACE && ft_isspace(*str))
			break ;
		str++;
	}
	// if (ltrim_opt)
		// return (ft_ltrim(str));
	return (str);
}


// int main(int argc, char **argv)
// {
// 	char *buffer;
// 	char *next;

// 	buffer = readline("test>");
// 	printf("buffer:%p\n", buffer);
// 	printf("buffer:%s\n", buffer);
// 	while (*buffer)
// 	{
// 		next = next_word(buffer);
// 		if (!next)
// 		{
// 			printf ("error\n");
// 			return (0);
// 		}
// 		printf("next:%p\n", next);
// 		printf("next:%s\n", next);
// 		printf("%ld\n", next - buffer);
// 		buffer = next;
// 	}
// }