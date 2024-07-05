#include "../minishell.h"

// stdout으로 없는 내용 쓰는 것 구현하면 마지막 exit code가 훼손될 수 있음.
built_in	select_builtin(int index)
{
	built_in	fp[7];

	fp[0] = ft_echo;
	fp[1] = ft_cd;
	fp[2] = ft_pwd;
	fp[3] = ft_export;
	fp[4] = ft_unset;
	fp[5] = ft_env;
	fp[6] = ft_exit;
	// printf("right function %d\n", index);
	return (fp[index]);
}
