/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabodaya <yabodaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 16:32:16 by yabodaya          #+#    #+#             */
/*   Updated: 2026/06/07 00:00:00 by yabodaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	open_infile(t_redir *redir)
{
	int	fd;

	fd = open(redir->target, O_RDONLY);
	if (fd < 0)
	{
		perror(redir->target);
		return (1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

static int	open_outfile(t_redir *redir)
{
	int	fd;

	if (redir->type == REDIR_APPEND)
		fd = open(redir->target, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(redir->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror(redir->target);
		return (1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static int	apply_heredoc(t_redir *redir)
{
	if (redir->fd < 0)
		return (1);
	dup2(redir->fd, STDIN_FILENO);
	close(redir->fd);
	redir->fd = -1;
	return (0);
}

static int	apply_one_redir(t_redir *redir, char **envp, int *exit_status)
{
	(void)envp;
	(void)exit_status;
	if (redir->type == REDIR_HEREDOC)
		return (apply_heredoc(redir));
	else if (redir->type == REDIR_IN)
		return (open_infile(redir));
	else
		return (open_outfile(redir));
}

int	setup_redirections(t_cmd *cmd, char **envp, int *exit_status)
{
	t_redir	*redir;

	redir = cmd->redirs;
	while (redir)
	{
		if (apply_one_redir(redir, envp, exit_status))
			return (1);
		redir = redir->next;
	}
	return (0);
}
