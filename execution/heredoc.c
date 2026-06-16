/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabodaya <yabodaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 21:09:32 by yabodaya          #+#    #+#             */
/*   Updated: 2026/06/07 00:00:00 by yabodaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	heredoc_write_line(t_hd_ctx *ctx, char *line)
{
	char	*expanded;

	if (ctx->quoted)
		expanded = ft_strdup(line);
	else
		expanded = expand_value(line, ctx->envp, 0, ctx->est);
	if (!expanded)
		expanded = ft_strdup("");
	if (expanded)
	{
		write(ctx->write_fd, expanded, ft_strlen(expanded));
		write(ctx->write_fd, "\n", 1);
		free(expanded);
	}
}

static void	heredoc_run_child(t_redir *redir, int wfd, char **envp, int est)
{
	t_hd_ctx	ctx;
	char		*line;

	ctx.write_fd = wfd;
	ctx.quoted = redir->quoted;
	ctx.envp = envp;
	ctx.est = est;
	set_signal_heredoc();
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, redir->target) == 0)
		{
			free(line);
			break ;
		}
		heredoc_write_line(&ctx, line);
		free(line);
	}
	close(wfd);
	exit(0);
}

static int	heredoc_make_tmp(char **name)
{
	static int	id;
	char		*num;
	int			fd;

	*name = NULL;
	num = ft_itoa(id++);
	if (!num)
		return (-1);
	*name = ft_strjoin("/tmp/.ms_hd_", num);
	free(num);
	if (!*name)
		return (-1);
	fd = open(*name, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
	{
		free(*name);
		*name = NULL;
	}
	return (fd);
}

int	collect_one_heredoc(t_redir *redir, char **envp, int *exit_status)
{
	char	*name;
	int		wfd;
	int		status;
	pid_t	pid;

	wfd = heredoc_make_tmp(&name);
	if (wfd < 0)
		return (perror("heredoc"), 1);
	set_signal_exec();
	pid = fork();
	if (pid < 0)
		return (close(wfd), unlink(name), free(name),
			set_signal_prompt(), perror("fork"), 1);
	if (pid == 0)
		heredoc_run_child(redir, wfd, envp, *exit_status);
	close(wfd);
	waitpid(pid, &status, 0);
	set_signal_prompt();
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		return (write(STDOUT_FILENO, "\n", 1), *exit_status = 130,
			g_signal = SIGINT, unlink(name), free(name), 1);
	return (redir->fd = open(name, O_RDONLY), unlink(name),
		free(name), 0);
}

int	collect_heredocs(t_cmd *cmd, char **envp, int *exit_status)
{
	t_redir	*redir;

	while (cmd)
	{
		redir = cmd->redirs;
		while (redir)
		{
			if (redir->type == REDIR_HEREDOC
				&& collect_one_heredoc(redir, envp, exit_status))
				return (1);
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (0);
}
