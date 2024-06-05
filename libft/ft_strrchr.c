/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 18:11:05 by yubshin           #+#    #+#             */
/*   Updated: 2023/11/03 17:11:52 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strrchr(const char *str, int c)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	if ((char) c == 0)
		return ((char *) str + i);
	while (i >= 0)
	{
		if (str[i] == (char) c)
			return ((char *) str + i);
		i--;
	}
	return (0);
}
