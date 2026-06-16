/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabodaya <yabodaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 14:39:44 by yabodaya          #+#    #+#             */
/*   Updated: 2026/06/02 21:09:32 by yabodaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	update_exit_status(int status, int *exit_status)
{
	if (WIFEXITED(status))
		*exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
			write(STDERR_FILENO, "Quit (core dumped)\n", 19);
		else if (WTERMSIG(status) == SIGINT)
			write(STDERR_FILENO, "\n", 1);
		*exit_status = 128 + WTERMSIG(status);
	}
}

void	pipe_child_dispatch(t_cmd *cmd, char **envp, int *exit_status)
{
	if (!cmd->args || !cmd->args[0])
		exit(0);
	if (is_builtin(cmd))
	{
		exec_builtin_core(cmd, &envp, exit_status);
		exit(*exit_status);
	}
	child_exec(cmd, envp);
}

static void	run_redirs_only(t_cmd *cmd, char **envp, int *exit_status)
{
	int	saved_in;
	int	saved_out;

	if (!cmd->redirs)
		return ;
	if (save_stdio(&saved_in, &saved_out))
		return ;
	if (setup_redirections(cmd, envp, exit_status))
		*exit_status = 1;
	else
		*exit_status = 0;
	restore_stdio(saved_in, saved_out);
}

void	execute(t_cmd *cmd, char ***envp, int *exit_status)
{
	if (!cmd)
		return ;
	if (collect_heredocs(cmd, *envp, exit_status))
		return ;
	if (!cmd->next)
	{
		if (!cmd->args || !cmd->args[0])
			run_redirs_only(cmd, *envp, exit_status);
		else if (is_builtin(cmd))
			exec_builtin(cmd, envp, exit_status);
		else
			exec_cmd(cmd, *envp, exit_status);
	}
	else
		pipe_exec(cmd, envp, exit_status);
}
