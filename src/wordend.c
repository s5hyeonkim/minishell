#include "../minishell.h"

char	*find_quotend(char *str, int flag);
int		ft_iswordhead(char chr);

char	*find_wordend(char *str) //word: quote, space(not_quote), pipe, direct 기준
{
	str = find_notspace(str);
	if (*str == SGL_QUOTE || *str == DBL_QUOTE) //quote문 처리
		str = find_quotend(str, ft_isquote(*str));
	else
		while (str && *str && ft_iswordhead(*str) == FALSE)
			str++;
	// printf("find_wordend:%s\n", str);
	return (str);
}

char	*find_quotend(char *str, int flag)
{
	int	sgl_open;
	int	dbl_open;

	sgl_open = 0;
	dbl_open = 0;
	while(*str)
	{
		if (*str == SGL_QUOTE && !sgl_open && dbl_open && dbl_open--)
			return(ft_strchr(str, DBL_QUOTE) + 1);
		else if (*str == SGL_QUOTE && !sgl_open && !dbl_open)
			sgl_open = TRUE;
		else if (*str == DBL_QUOTE && !dbl_open && sgl_open && sgl_open--)
			return(ft_strchr(str, SGL_QUOTE) + 1);
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

int	ft_iswordhead(char chr)
{
	if (ft_isspace(chr) == TRUE \
	|| ft_ispipe(chr) == TRUE \
	|| ft_isquote(chr) != FALSE \
	|| ft_isredirect(chr) == TRUE)
		return (TRUE);
	return (FALSE);
}