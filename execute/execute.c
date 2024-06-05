#include "../minishell.h"

// stdout으로 없는 내용 쓰는 것 구현하면 마지막 exit code가 훼손될 수 있음.

t_process	*create_process(size_t size)
{
	t_process	*new;

	new = malloc(sizeof(t_process) * size);
	if (new)
		ft_memset(new, 0, sizeof(t_process) * size);
	return (new);
}

void set_process(t_exec *exec)
{
	exec->p = create_process(exec->size);
	if (exec->p)
		exit_process(exec, "", MALLOC_FAILED);
}

int	execute_process(t_exec *exec, char *envp[])
{
	set_process(exec);
	
}

int tokenization(t_exec *e, int size, char *argv[])
{
	int	index;

	index = 0;
	e->t = malloc(sizeof(t_token) * --size);
	if (!e->t)
		return (EXIT_FAILURE);
	while(index < size)
	{
		e->t[index].cmd = ft_strdup(argv[index]);
		e->t[index].type = CMD;
		index++;
	}
	e->size = size;
	return (EXIT_SUCCESS);
}

int main(int argc, char *argv[], char *envp[])
{
	t_exec	e;

	if (argc == 1)
		return (0);
	ft_memset(&e, 0, sizeof(t_exec));
	if (tokenization(&e, argc, argv) || execute_process(&e, envp))
		return (0);
}