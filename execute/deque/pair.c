#include "deque.h"

char	*ft_pairjoin(char *key, char mid, char *val)
{
	char	*temp;
	char	*ret;
    char    m[2];

	ret = NULL;
    m[0] = mid;
    m[1] = 0;
	temp = ft_strjoin(key, m);
	if (!temp)
		return (NULL);
	ret = ft_strjoin(temp, val);
	free(temp);
	return (ret);
}

char    *get_key_str(char *str)
{
    char    *key;

    if (ft_strchr(str, '='))
        key = ft_substr(str, 0, ft_strchr(str, '=') - str);
    else
        key = ft_strdup(str);
    return (key);
}

char    *get_val_str(char *str)
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
    keyval->key = get_key_str(str);
    keyval->val = get_val_str(str);
	if (!keyval->key || !keyval->val)
	{
		free_keyval(*keyval);
		keyval->key = NULL;
		keyval->val = NULL;
		return (EXTRA_ERROR);
	}
	return (EXIT_SUCCESS);
}
