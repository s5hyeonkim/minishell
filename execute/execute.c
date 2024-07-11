#include "../minishell.h"
#include "execute.h"
// stdout으로 없는 내용 쓰는 것 구현하면 마지막 exit code가 훼손될 수 있음.
built_in	find_builtin(int index)
{
	built_in	fp[7];

	fp[_ECHO] = ft_echo;
	fp[_CD] = ft_cd;
	fp[_PWD] = ft_pwd;
	fp[_EXPORT] = ft_export;
	fp[_UNSET] = ft_unset;
	fp[_ENV] = ft_env;
	fp[_EXIT] = ft_exit;
	// printf("right function %d\n", index);
	return (fp[index]);
}