/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 11:25:39 by sounania          #+#    #+#             */
/*   Updated: 2026/04/24 18:32:17 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_cmd	*create_command(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirs = NULL;
	cmd->next = NULL;
	return (cmd);
}

int	add_redir(t_cmd *cmd, t_redir_type type, char *target, int quoted)
{
	t_redir	*node;
	t_redir	*last;

	node = malloc(sizeof(t_redir));
	if (!node)
		return (0);
	node->type = type;
	node->target = target;
	node->quoted = quoted;
	node->fd = -1;
	node->next = NULL;
	if (!cmd->redirs)
		cmd->redirs = node;
	else
	{
		last = cmd->redirs;
		while (last->next)
			last = last->next;
		last->next = node;
	}
	return (1);
}

void	free_redirs(t_redir *redir)
{
	t_redir	*tmp;

	while (redir)
	{
		tmp = redir;
		redir = redir->next;
		if (tmp->fd >= 0)
			close(tmp->fd);
		if (tmp->target)
			free(tmp->target);
		free(tmp);
	}
}

void	free_command(t_cmd *cmd)
{
	int	i;

	if (!cmd)
		return ;
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
		{
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
	}
	free_redirs(cmd->redirs);
	free(cmd);
}

void	free_commands(t_cmd *cmds)
{
	t_cmd	*temp;

	while (cmds)
	{
		temp = cmds;
		cmds = cmds->next;
		free_command(temp);
	}
}
