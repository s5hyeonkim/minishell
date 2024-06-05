#include "minishell.c"

//free function

void exit_process(t_exec *e, char *str, t_err err_code)
{
	if (err_code == CMD_NOT_FOUND)
	{
		ft_putstr_fd("zsh: command not found", STDERR_FILENO);
		ft_putstr_fd(str, STDERR_FILENO);
	}
	else if (err_code == FILE_NOT_EXIST)
	{
		ft_putstr_fd(perror(""), STDERR_FILENO);
		ft_putstr_fd(str, STDERR_FILENO);
	}
	else
		ft_putstr_fd(perror(""), STDERR_FILENO);
	if (err_code && err_code != 127)
		err_code = 1;
	//free
	//free_function(e);
	exit(err_code);
}
