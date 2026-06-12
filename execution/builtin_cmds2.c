/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmds2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabodaya <yabodaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 00:53:08 by yabodaya          #+#    #+#             */
/*   Updated: 2026/06/02 21:09:32 by yabodaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	pwd_bi(t_cmd *cmd, int *exit_status)
{
	char	*cwd;

	(void)cmd;
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("pwd");
		*exit_status = 1;
		return ;
	}
	write(STDOUT_FILENO, cwd, ft_strlen(cwd));
	write(STDOUT_FILENO, "\n", 1);
	free(cwd);
	*exit_status = 0;
}

static void	cd_update_pwd(char ***envp)
{
	char	*cwd;
	char	*old_pwd;

	old_pwd = env_get_value(*envp, "PWD");
	if (old_pwd)
	{
		update_env_variable(envp, "OLDPWD", old_pwd);
		free(old_pwd);
	}
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		update_env_variable(envp, "PWD", cwd);
		free(cwd);
	}
}

static int	cd_to(char *path, char ***envp, int *exit_status)
{
	if (chdir(path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		perror(path);
		*exit_status = 1;
		return (1);
	}
	cd_update_pwd(envp);
	*exit_status = 0;
	return (0);
}

void	cd_bi(t_cmd *cmd, char ***envp, int *exit_status)
{
	char	*home;

	if (cmd->args[1] && cmd->args[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		*exit_status = 1;
		return ;
	}
	if (!cmd->args[1])
	{
		home = env_get_value(*envp, "HOME");
		if (!home)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			*exit_status = 1;
			return ;
		}
		cd_to(home, envp, exit_status);
		free(home);
		return ;
	}
	cd_to(cmd->args[1], envp, exit_status);
}
