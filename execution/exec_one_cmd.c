/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_one_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabodaya <yabodaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 12:59:30 by yabodaya          #+#    #+#             */
/*   Updated: 2026/06/02 21:09:32 by yabodaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	cmd_not_found(char *cmd)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": command not found\n", 2);
	exit(127);
}

static void	cmd_permission(char *cmd, char *path)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": Permission denied\n", 2);
	free(path);
	exit(126);
}

static void	check_directory(char *cmd, char *path)
{
	struct stat	st;

	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": is a directory\n", 2);
		free(path);
		exit(126);
	}
}

void	child_exec(t_cmd *cmd, char **envp)
{
	char	*path;

	set_signal_child();
	path = get_path(cmd->args[0], envp);
	if (!path)
		cmd_not_found(cmd->args[0]);
	check_directory(cmd->args[0], path);
	if (access(path, X_OK) != 0)
		cmd_permission(cmd->args[0], path);
	execve(path, cmd->args, envp);
	perror("minishell");
	free(path);
	exit(126);
}

void	exec_cmd(t_cmd *cmd, char **envp, int *exit_status)
{
	pid_t	pid;
	int		status;
	int		dummy;

	if (!cmd || !cmd->args || !cmd->args[0])
		return ;
	dummy = *exit_status;
	pid = fork();
	if (pid < 0)
	{
		perror("minishell");
		*exit_status = 1;
		return ;
	}
	if (pid == 0)
	{
		if (setup_redirections(cmd, envp, &dummy))
			exit(1);
		child_exec(cmd, envp);
	}
	set_signal_exec();
	waitpid(pid, &status, 0);
	set_signal_prompt();
	update_exit_status(status, exit_status);
}
