/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 16:01:35 by yubshin           #+#    #+#             */
/*   Updated: 2023/11/03 17:07:16 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static char	*ft_put(long long n, size_t nlen, char *res)
{
	size_t	i;

	i = 0;
	if (n < 0)
	{
		res[0] = '-';
		n = n * -1;
		i++;
	}
	while (i < nlen)
	{
		if (res[0] == '-')
			res[nlen - i] = n % 10 + '0';
		else
			res[nlen - 1 - i] = n % 10 + '0';
		n = n / 10;
		i++;
	}
	res[nlen] = '\0';
	return (res);
}	

char	*ft_itoa(int n)
{
	char		*res;
	long long	n2;
	size_t		nlen;

	n2 = n;
	nlen = 1;
	if (n2 < 0)
	{
		n2 = n2 * -1;
		nlen++;
	}
	while (n2 / 10 > 0)
	{
		nlen++;
		n2 = n2 / 10;
	}
	res = (char *)malloc(sizeof (char) * (nlen + 1));
	if (res == NULL)
		return (0);
	res[0] = 0;
	ft_put(n, nlen, res);
	return (res);
}
