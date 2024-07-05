#include "deque.h"

char	*ft_pairjoin(t_pairs keyval)
{
	char	*temp;
	char	*ret;
    char    mid[2];

	ret = NULL;
    mid[0] = keyval.mid;
    mid[1] = 0;
	temp = ft_strjoin(keyval.key, mid);
	if (!temp)
		return (NULL);
	ret = ft_strjoin(temp, keyval.val);
	free(temp);
	return (ret);
}

char    *get_key_strs(char *str)
{
    char    *key;

    if (ft_strchr(str, '='))
        key = ft_substr(str, 0, ft_strchr(str, '=') - str - 1);
    else
        key = ft_strdup(str);
    return (key);
}

char    *get_value_strs(char *str)
{
    char    *val;

    if (ft_strchr(str, '='))
        val = ft_substr(str, ft_strchr(str, '=') - str + 1, ft_strlen(str));
    else
        val = ft_strdup("");
    return (val);
}

int	set_keyval(char *str, t_pairs *keyval)
{
	if (ft_strchr(str, '='))
		keyval->mid = '=';
    else
        keyval->mid = 0;
    keyval->key = get_key_strs(str);
    keyval->val = get_value_strs(str);
	if (!keyval->key || !keyval->val)
	{
		free_keyval(*keyval);
		keyval->key = NULL;
		keyval->val = NULL;
		return (MALLOC_FAILED);
	}
	return (EXIT_SUCCESS);
}
