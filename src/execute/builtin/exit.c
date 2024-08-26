/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubshin <yubshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 19:31:56 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/21 17:17:10 by yubshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execute.h"

static char	*swap_str(char *s1)
{
	int		end;
	int		start;
	char	rep;

	start = 0;
	end = ft_strlen(s1) - 1;
	while (start < end)
	{
		rep = s1[start];
		s1[start] = s1[end];
		s1[end] = rep;
		start++;
		end--;
	}
	return (s1);
}

static int	is_equal(long n, char *str)
{
	char	ch[21];
	int		index;	

	index = 0;
	ft_memset(ch, 0, sizeof(char) * 21);
	while (!index || n)
	{
		if (n < 0)
		{
			ch[index++] = '0' - n % 10;
			ch[index] = '-';
		}
		else
			ch[index++] = '0' + n % 10;
		n /= 10;
	}
	swap_str(ch);
	if (ft_memcmp(str, ch, ft_strlen(str)))
		return (FALSE);
	return (TRUE);
}

static int	ft_is_space(char ch)
{
	if (ch >= 9 && ch <= 13)
		return (TRUE);
	if (ch == 32)
		return (TRUE);
	return (FALSE);
}

long	ft_atol(const char *str)
{
	long	sign;
	long	result;

	result = 0;
	sign = 1;
	while (ft_is_space(*str))
		str++;
	if (*str == '+')
		str++;
	else if (*str == '-')
	{
		sign = -1;
		str++;
	}
	while (ft_isdigit(*str))
	{
		result = result * 10 + *str - '0';
		str++;
	}
	return (result * sign);
}

int	ft_exit(t_process p, t_data *d)
{
	int	index;

	(void) d;
	index = 1;
	if (p.exec.args[index] && !ft_memcmp(p.exec.args[index], "--", 3))
		index++;
	if (p.exec.args[index])
	{
		g_status = ft_atol(p.exec.args[index]);
		if (!is_equal(g_status, p.exec.args[index]))
		{
			g_status = 255;
			handle_error(p.exec.args[0], p.exec.args[index], NOT_NUM);
			return (EXIT_SUCCESS);
		}
		else if (p.exec.args[index + 1])
		{
			g_status = EXIT_FAILURE;
			write(STDERR_FILENO, "minishell: exit: too many arguments\n", 36);
			return (EXIT_FAILURE);
		}
	}
	return (EXIT_SUCCESS);
}
