/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabodaya <yabodaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 16:06:22 by yabodaya          #+#    #+#             */
/*   Updated: 2026/06/02 21:09:32 by yabodaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	save_stdio(int *saved_in, int *saved_out)
{
	*saved_in = dup(STDIN_FILENO);
	*saved_out = dup(STDOUT_FILENO);
	if (*saved_in == -1 || *saved_out == -1)
		return (1);
	return (0);
}

void	restore_stdio(int saved_in, int saved_out)
{
	dup2(saved_in, STDIN_FILENO);
	dup2(saved_out, STDOUT_FILENO);
	close(saved_in);
	close(saved_out);
}

int	is_builtin(t_cmd *cmd)
{
	const char	*builtins[] = {"echo", "cd", "pwd", "export",
		"unset", "env", "exit", NULL};
	int			i;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	i = 0;
	while (builtins[i])
	{
		if (ft_strcmp(cmd->args[0], builtins[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

void	exec_builtin_core(t_cmd *cmd, char ***envp, int *exit_status)
{
	if (!ft_strcmp(cmd->args[0], "echo"))
		echo_bi(cmd, exit_status);
	else if (!ft_strcmp(cmd->args[0], "cd"))
		cd_bi(cmd, envp, exit_status);
	else if (!ft_strcmp(cmd->args[0], "pwd"))
		pwd_bi(cmd, exit_status);
	else if (!ft_strcmp(cmd->args[0], "export"))
		export_bi(cmd, envp, exit_status);
	else if (!ft_strcmp(cmd->args[0], "unset"))
		unset_bi(cmd, envp, exit_status);
	else if (!ft_strcmp(cmd->args[0], "env"))
		env_bi(cmd, *envp, exit_status);
	else if (!ft_strcmp(cmd->args[0], "exit"))
		exit_bi(cmd, exit_status);
}

void	exec_builtin(t_cmd *cmd, char ***envp, int *exit_status)
{
	int	saved_in;
	int	saved_out;

	if (save_stdio(&saved_in, &saved_out))
		return ;
	if (setup_redirections(cmd, *envp, exit_status))
	{
		restore_stdio(saved_in, saved_out);
		*exit_status = 1;
		return ;
	}
	exec_builtin_core(cmd, envp, exit_status);
	restore_stdio(saved_in, saved_out);
}
