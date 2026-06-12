/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabodaya <yabodaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 13:45:10 by yabodaya          #+#    #+#             */
/*   Updated: 2026/06/02 21:09:32 by yabodaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	pipe_run_child(t_cmd *cmd, t_pipe_ctx *ctx)
{
	int	dummy;

	dummy = *ctx->exit_status;
	set_signal_child();
	dup2(ctx->in_fd, STDIN_FILENO);
	if (cmd->next)
		dup2(ctx->pipefd[1], STDOUT_FILENO);
	if (cmd->next)
	{
		close(ctx->pipefd[0]);
		close(ctx->pipefd[1]);
	}
	if (ctx->in_fd != STDIN_FILENO)
		close(ctx->in_fd);
	if (setup_redirections(cmd, ctx->envp, &dummy))
		exit(1);
	pipe_child_dispatch(cmd, ctx->envp, &dummy);
}

static void	pipe_handle_parent(t_pipe_ctx *ctx, t_cmd *cmd)
{
	if (cmd->next)
	{
		close(ctx->pipefd[1]);
		if (ctx->in_fd != STDIN_FILENO)
			close(ctx->in_fd);
		ctx->in_fd = ctx->pipefd[0];
	}
}

static pid_t	pipe_fork_one(t_cmd *cmd, t_pipe_ctx *ctx)
{
	pid_t	pid;

	if (cmd->next && pipe(ctx->pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
		pipe_run_child(cmd, ctx);
	else
		pipe_handle_parent(ctx, cmd);
	return (pid);
}

static void	pipe_wait_all(pid_t last_pid, int *exit_status)
{
	int	status;

	set_signal_exec();
	if (last_pid > 0)
		waitpid(last_pid, &status, 0);
	while (wait(NULL) > 0)
		;
	set_signal_prompt();
	if (last_pid > 0)
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
}

void	pipe_exec(t_cmd *cmd, char ***envp, int *exit_status)
{
	t_pipe_ctx	ctx;
	pid_t		last_pid;

	ctx.in_fd = STDIN_FILENO;
	ctx.exit_status = exit_status;
	ctx.envp = *envp;
	last_pid = -1;
	while (cmd)
	{
		last_pid = pipe_fork_one(cmd, &ctx);
		if (last_pid == -1)
			break ;
		cmd = cmd->next;
	}
	if (ctx.in_fd != STDIN_FILENO)
		close(ctx.in_fd);
	pipe_wait_all(last_pid, exit_status);
}
