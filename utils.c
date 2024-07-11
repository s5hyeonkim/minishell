# include "minishell.h"
int	is_equalquote(char prev, char next)
{
	if (prev != next)
		return (FALSE);
	if (prev != '\'' && prev != '\"')
		return (FALSE);
	return (TRUE);
}

int	set_quote(t_pairs *keyval, char c)
{
	keyval->mid = c;
	return (TRUE);
}

int	is_close(t_shell *shell, char *str)
{
	t_deques	*rec;
	int			index;
	t_pairs		keyval;
	int			ret;

	rec = create_deques();
	keyval.mid = -1;
	if (!rec || push_back(rec, keyval))
		exit_process(shell, NULL, MALLOC_FAILED);
	index = -1;
	ret = FALSE;
	while(str[++index])
	{
		if (is_equalquote(rec->tail->keyval.mid, str[index]))
			free_deque(pop_back(rec));
		else if (set_quote(&keyval, str[index]) && push_back(rec, keyval))
		{
			free_deques(&rec);
			exit_process(shell, NULL, MALLOC_FAILED);
		}
	}
	if (rec->tail->keyval.mid == -1)
		ret = TRUE;
	free_deques(&rec);
	return (ret);
}