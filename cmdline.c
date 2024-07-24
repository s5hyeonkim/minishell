#include "minishell.h"

char	*get_pathcmd(char **paths, char *cmd)
{
	int		index;
	char	*ret;

	index = 0;
	while (paths[index])
	{
		ret = ft_pairjoin(paths[index++],'/', cmd);
		if (!ret || !access(ret, X_OK))
			return (ret);
		free(ret);
	}
	return (ft_strdup(cmd));
}

char	*get_cmdpath(char **paths, char *cmd)
{
	char	*ret;

	if (is_builtin(cmd))
		ret = ft_strdup(cmd);
	else
		ret = get_pathcmd(paths, cmd);
	return (ret);
}

int	is_valid_quotation(size_t *start, int *open1, int open2)
{
	*open1 = !(*open1);
	if (!open2)
	{
		if (!(*open1))
			(*start)++;
		return (TRUE);
	}
	return (FALSE);
}

// redirection 추가 필요
int	check_quotation_flag(int *flag, int flag2)
{
	*flag = !(*flag);
	if (!flag2)
		return (TRUE);
	return (FALSE);
}

void	parsing_cmd(char *cmd, size_t *start, size_t *end)
{
	size_t		size;
	int			flag;
	int			flag1;

	size = *start;
	flag = 0;
	flag1 = 0;
	while (cmd[size] == ' ')
		size++;
	*start = size;
	while (cmd[size])
	{
		if (cmd[size] == '\'' && check_quotation_flag(&flag, flag1))
			size++;
		if (cmd[size] == '\"' && check_quotation_flag(&flag1, flag))
			size++;
		if (!flag1 && !flag && cmd[size] == ' ')
			break ;
		size++;
	}
	*end = size;
}

int	set_parsing_deques(t_deques *deqs, char *cmd)
{
	size_t		fb[2];
	size_t		len;
	char		*str;
	t_map		keyval;

	len = ft_strlen(cmd);
	ft_memset(fb, 0, sizeof(size_t) * 2);
	while (fb[0] < len)
	{
		parsing_cmd(cmd, &fb[0], &fb[1]);
		if (fb[0] >= len)
			break ;
		str = ft_substr(cmd, fb[0], fb[1] - fb[0]);
		if (!str || set_keyval(&keyval, str, 0, "") || push_back(deqs, keyval))
		{
			free(str);
			free_map(&keyval);
			return (EXTRA_ERROR);
		}
		free(str);
		fb[0] = ++fb[1];
	}
	return (EXIT_SUCCESS);
}

char	**get_cmdargs(char *cmd)
{
	t_deques	*deqs;
	char		**strs;

	deqs = create_deques();
	if (!deqs || set_parsing_deques(deqs, cmd))
	{
		free_deques(&deqs);
		return (NULL);
	}
	strs = deqtostrs(deqs);
	// printf("%s\n", strs[0]);
	free_deques(&deqs);
	return (strs);
}
