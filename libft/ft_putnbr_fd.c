/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 12:55:45 by yubshin           #+#    #+#             */
/*   Updated: 2023/11/03 17:09:55 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static int	ft_putnbr(long long n, int fd)
{
	char	digit;

	if (n > 0)
	{
		ft_putnbr(n / 10, fd);
		digit = n % 10 + '0';
		write(fd, &digit, 1);
	}
	return (0);
}

void	ft_putnbr_fd(int n, int fd)
{
	long long	n2;

	n2 = (long long)n;
	if (n2 < 0)
	{
		write(fd, "-", 1);
		n2 *= -1;
	}
	if (n2 == 0)
		write(fd, "0", 1);
	ft_putnbr(n2, fd);
}
