/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 17:23:02 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/26 17:23:44 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../execute.h"

static int	is_plusval(char *str)
{
	char	*target;

	target = ft_strnstr(str, "+=", ft_strlen(str));
	if (!target)
		return (FALSE);
	return (TRUE);
}

static int	is_noreplace(t_deques *envps, t_map keyval)
{
	if (keyval.state != ENV && find_deq(envps, keyval.key))
		return (TRUE);
	return (FALSE);
}

static int	concat_val(t_deques *envps, t_map *keyval)
{
	char	*line;
	char	*tail;

	tail = get_val_deq(envps, keyval->key);
	if (!tail)
		return (EXTRA_ERROR);
	line = ft_strjoin(tail, keyval->val);
	free(tail);
	free(keyval->val);
	keyval->val = line;
	if (!line)
		return (EXTRA_ERROR);
	return (EXIT_SUCCESS);
}

int	add_val(t_deques *envps, t_map *keyval, char *str)
{
	if (is_noreplace(envps, *keyval))
		return (EXIT_SUCCESS);
	if (is_plusval(str) && concat_val(envps, keyval))
		return (EXTRA_ERROR);
	if (keyval->mid)
		return (replace_back(envps, keyval->key, keyval->val, ENV));
	return (replace_back(envps, keyval->key, keyval->val, EXPORT));
}
