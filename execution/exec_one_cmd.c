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

static void	cmd_error(char *cmd, char *path, char *msg, int code)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(msg, 2);
	if (path)
		free(path);
	exit(code);
}

static void	check_directory(char *cmd, char *path)
{
	struct stat	st;

	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
		cmd_error(cmd, path, ": Is a directory\n", 126);
}

static void	exec_sh_fallback(char *path, char **args, char **envp)
{
	char	**sh;
	int		n;
	int		i;

	n = 0;
	while (args[n])
		n++;
	sh = malloc(sizeof(char *) * (n + 2));
	if (!sh)
		exit(126);
	sh[0] = "/bin/sh";
	sh[1] = path;
	i = 1;
	while (args[i])
	{
		sh[i + 1] = args[i];
		i++;
	}
	sh[i + 1] = NULL;
	execve("/bin/sh", sh, envp);
	free(sh);
	exit(126);
}

void	child_exec(t_cmd *cmd, char **envp)
{
	char	*path;

	set_signal_child();
	path = get_path(cmd->args[0], envp);
	if (!path && ft_strchr(cmd->args[0], '/'))
		cmd_error(cmd->args[0], NULL, ": No such file or directory\n", 127);
	if (!path)
		cmd_error(cmd->args[0], NULL, ": command not found\n", 127);
	check_directory(cmd->args[0], path);
	if (access(path, X_OK) != 0)
		cmd_error(cmd->args[0], path, ": Permission denied\n", 126);
	execve(path, cmd->args, envp);
	if (errno == ENOEXEC)
		exec_sh_fallback(path, cmd->args, envp);
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
