# include "minishell.h"
int	is_quote(char c)
{
	if (c == '\'' || c == '\"')
		return (TRUE);
	return (FALSE);
}

int	is_close(t_exec *info, char *str)
{
	t_deques	*rec;
	t_deque		*pop;
	int			index;
	int			ret;

	rec = create_deques();
	if (!rec || push_back_char(rec, -1))
		exit_process(info, NULL, MALLOC_FAILED);
	while(str[index])
	{
		if (is_quote(str[index]) && rec->tail->mid == str[index])
		{
			pop = pop_back(rec);
			free_deque(pop);
		}
		else if (is_quote(str[index]) && push_back_char(rec, str[index]))
			exit_process(info, NULL, MALLOC_FAILED);
		index++;
	}
	if (rec->tail->mid == -1)
		ret = TRUE;
	free_deques(&rec);	
	return (FALSE);
}