#include "ft_err.h"

void	print_objerr(char *obj, int code)
{
	if (code == CMD_NOT_FOUND)
		printf("minishell: %s : command not found\n", obj);
	else if (code == INVALID_ARGV)
		printf("minishell: %s: invalid arguments or options\n", obj);
	else
		printf("minishell: %s: %s\n", obj, strerror(errno));
}

void	print_err(int code)
{
	if (code == INVALID_ARGV)
		printf("minishell: invalid arguments\n");
	else
		perror("minishell: ");
}

int	handle_error(char *exec, char *obj, int code)
{
	int	stdout_fd;

	stdout_fd = dup(STDOUT_FILENO);
	dup2(STDERR_FILENO, STDOUT_FILENO);
	if (obj)
		printf("minishell: %s : command not found\n", obj);
	else if (exec)
		print_objerr(exec, code);
	else
		print_err(code);
	dup2(stdout_fd, STDOUT_FILENO);
	if (code == 127)
		return (code);
	else if (code)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
