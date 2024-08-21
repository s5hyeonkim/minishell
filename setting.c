/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setting.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohykim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 07:09:55 by sohykim           #+#    #+#             */
/*   Updated: 2024/08/19 16:51:45 by sohykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int		set_env_paths(t_data *d)
{
	char	**envp;

	free_strs(d->paths);
	d->paths = NULL;
	envp = deqtostrs(d->envps->head);
	if (envp)
		d->paths = get_env_paths(envp);
	free_strs(envp);
	if (d->paths)
		return (EXIT_SUCCESS);
	return (EXTRA_ERROR);
}

static void	set_depth(t_shell *shell)
{
	int		depth;
	char	*itoa;

	depth = get_depth(read_val_deq(shell->data.envps, "SHLVL")) + 1;
	if (depth == 1000)
		itoa = ft_strdup("");
	else
		itoa = ft_itoa(depth);
	if (!itoa || replace_back(shell->data.envps, "SHLVL", '=', itoa))
	{
		free(itoa);
		exit_process(shell, NULL, EXTRA_ERROR);
	}
	free(itoa);
}

static void	set_pwd_path(t_shell *shell)
{
	char	*cwd;

	cwd = ft_strdup(shell->data.lcwd);
	if (!cwd || replace_back(shell->data.envps, "OLDPWD", 0, "") || \
	replace_back(shell->data.envps, "PWD", '=', cwd))
	{
		free(cwd);
		exit_process(shell, NULL, EXTRA_ERROR);
	}
	free(cwd);
}

static void	set_data(t_shell *shell, char *envp[])
{
	shell->data.envps = strstodeq(envp);
	if (!shell->data.envps || set_env_paths(&shell->data) || set_cwd(&shell->data.lcwd))
		exit_process(shell, NULL, EXTRA_ERROR);
	set_depth(shell);
	set_pwd_path(shell);
}

void	set_shell(t_shell *shell, char *envp[])
{
	ft_memset(shell, 0, sizeof(t_shell));
	set_data(shell, envp);
}
