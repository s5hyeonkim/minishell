#include "minishell.h"

void	set_data(t_shell *shell, char *envp[])
{
	shell->data.envps = strstodeq(envp);
	shell->data.paths = get_env_paths(envp);
	if (shell->data.envps)
	{
		shell->data.home = ft_strdup(read_val_deq(shell->data.envps, "HOME"));
		shell->data.user = ft_strdup(read_val_deq(shell->data.envps, "USER"));
	}
	if (!shell->data.paths || !shell->data.envps || !shell->data.home || !shell->data.user)
		exit_process(shell, NULL, EXTRA_ERROR);
}

void	set_shell(t_shell *shell, char *envp[])
{
	ft_memset(shell, 0, sizeof(t_shell));
	set_data(shell, envp);
}