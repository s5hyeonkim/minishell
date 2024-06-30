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
	while (*str >= '0' && *str <= '9')
	{
		pre = result;
		result = result * 10 + *str - '0';
		str++;
	}
	return (result * sign);
}

// exit 하기 전에 free 하자
int ft_exit(t_exec *info, t_process p)
{
    long    exit_code;
    int		stdout_fd;

	exit_code = 255;
	ft_putstr_fd("exit\n", STDERR_FILENO);
    if (p.args[1])
    {
        exit_code = ft_atol(p.args[1]);
        if (!is_equal(exit_code, p.args[1]))
        {
			stdout_fd = dup(STDOUT_FILENO);
			dup2(STDERR_FILENO, STDOUT_FILENO);
			printf("minishell: exit: %s: numeric argument required\n", p.args[1]);
			dup2(stdout_fd, STDOUT_FILENO);
			exit_code = 255;
        }
		else if (p.args[2])
			return (INVALID_ARGV);
    }
	free_info(*info);
    exit(exit_code);
	return (0);
}