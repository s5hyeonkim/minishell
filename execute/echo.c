#include "../minishell.h"

// $? 환경변수 기능 추가 필요
int	is_no_nl(char *str)
{
	if (str && !ft_memcmp(str, "-n", 3))		
		return (TRUE);
	return (FALSE);
}

char	*read_val_strs(char *strs[], char *key)
{
	char    *ret;	
	int     index;
	size_t	size;

	index = 0;
	ret = NULL;
	if (!strs)
		return (NULL);
	while (strs[index])
	{
		size = ft_strchr(strs[index], '=') - strs[index];
		if (!ft_memcmp(strs[index], key, size) && ft_strlen(key) == size)
		{
		    ret = strs[index] + size + 1;
			break ;
		}
		index++;
	}
	return (ret);
}

int	ft_echo(t_exec *info, t_process p)
{
	int 	index;

	//envs 각자 구하기.
	(void) info;
	index = 1;
	if (is_no_nl(p.args[1]))
		index++;
	while (p.args[index])
	{
		ft_putstr_fd(p.args[index], 1);
		if (p.args[++index])
			ft_putchar_fd(' ', 1);
	}
	if (!is_no_nl(p.args[1]))
		ft_putchar_fd('\n', 1);
	return (EXIT_SUCCESS);
}