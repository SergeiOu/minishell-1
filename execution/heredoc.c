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
			return (free(line));
		heredoc_write_line(&ctx, line);
		free(line);
	}
}

static int	heredoc_parent(t_redir *redir, int *pipefd, pid_t pid, int *est)
{
	int	status;

	close(pipefd[1]);
	waitpid(pid, &status, 0);
	set_signal_prompt();
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		return (write(STDOUT_FILENO, "\n", 1), *est = 130,
			g_signal = SIGINT, close(pipefd[0]), 1);
	redir->fd = pipefd[0];
	return (0);
}

int	collect_one_heredoc(t_redir *redir, char **envp, int *exit_status)
{
	int		pipefd[2];
	pid_t	pid;

	if (pipe(pipefd) == -1)
		return (perror("pipe"), 1);
	set_signal_exec();
	pid = fork();
	if (pid < 0)
		return (close(pipefd[0]), close(pipefd[1]),
			set_signal_prompt(), perror("fork"), 1);
	if (pid == 0)
	{
		close(pipefd[0]);
		heredoc_run_child(redir, pipefd[1], envp, *exit_status);
		close(pipefd[1]);
		exit(0);
	}
	return (heredoc_parent(redir, pipefd, pid, exit_status));
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
