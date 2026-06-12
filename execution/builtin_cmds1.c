/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmds1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabodaya <yabodaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 00:52:44 by yabodaya          #+#    #+#             */
/*   Updated: 2026/06/02 21:09:32 by yabodaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	env_bi(t_cmd *cmd, char **envp, int *exit_status)
{
	int	i;

	(void)cmd;
	i = 0;
	while (envp && envp[i])
	{
		if (ft_strchr(envp[i], '='))
		{
			write(STDOUT_FILENO, envp[i], ft_strlen(envp[i]));
			write(STDOUT_FILENO, "\n", 1);
		}
		i++;
	}
	*exit_status = 0;
}

static void	exit_print_err(const char *arg)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd((char *)arg, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
}

void	exit_bi(t_cmd *cmd, int *exit_status)
{
	long	code;

	ft_putstr_fd("exit\n", 1);
	if (!cmd->args[1])
		exit(*exit_status);
	if (!is_numeric(cmd->args[1]))
	{
		exit_print_err(cmd->args[1]);
		exit(2);
	}
	if (cmd->args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		*exit_status = 1;
		return ;
	}
	code = ft_atol(cmd->args[1]);
	exit((unsigned char)code);
}

void	unset_bi(t_cmd *cmd, char ***envp, int *exit_status)
{
	int	i;

	i = 1;
	while (cmd->args[i])
	{
		unset_process_arg(envp, cmd->args[i]);
		i++;
	}
	*exit_status = 0;
}

void	export_bi(t_cmd *cmd, char ***envp, int *exit_status)
{
	int	i;

	if (!cmd->args[1])
	{
		export_print_all(*envp);
		*exit_status = 0;
		return ;
	}
	i = 1;
	*exit_status = 0;
	while (cmd->args[i])
	{
		export_handle_arg(envp, cmd->args[i], exit_status);
		i++;
	}
}
