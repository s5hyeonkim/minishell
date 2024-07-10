#include "../minishell.h"

// 숫자 아닐때 에러 출력하며 그냥 exit (default = 255)
static char     *swap_str(char   *s1)
{
        int             end;
        int             start;
        char    rep;

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

int	is_equal(long n, char *str)
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
	long	pre;

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
		pre = result;
		result = result * 10 + *str - '0';
		str++;
	}
	return (result * sign);
}

// --처리 필요
int ft_exit(t_exec *info, t_process p)
{
    long    exit_code;
	int		index;

	index = 1;
	exit_code = info->status;
	ft_putstr_fd("exit\n", STDERR_FILENO);
	if (p.args[index] && !ft_memcmp(p.args[index], "--", 3))
		index++;
    if (p.args[index])
    {
        exit_code = ft_atol(p.args[index]);
        if (!is_equal(exit_code, p.args[index]))
        {
			handle_error(p.args[0], p.args[index], NOT_NUM);
			exit_code = 255;
        }
		else if (p.args[index + 1])
			return (handle_error(p.args[0], NULL, INVALID_ARGV));
    }
	free_info(*info);
    exit(exit_code);
	return (0);
}