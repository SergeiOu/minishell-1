/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_builder.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabodaya <yabodaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 11:29:09 by sounania          #+#    #+#             */
/*   Updated: 2026/02/06 14:18:18 by yabodaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	process_token(t_cmd *cmd, t_token **tokens, int *arg_index)
{
	if ((*tokens)->type == TOKEN_WORD)
	{
		if (!add_split_word(cmd, *tokens, arg_index))
			return (0);
	}
	else if ((*tokens)->type == TOKEN_REDIR_IN)
	{
		if (!handle_redir_in(cmd, tokens))
			return (0);
	}
	else if ((*tokens)->type == TOKEN_REDIR_OUT
		|| (*tokens)->type == TOKEN_REDIR_APPEND)
	{
		if (!handle_redir_out(cmd, tokens))
			return (0);
	}
	else if ((*tokens)->type == TOKEN_REDIR_HEREDOC)
	{
		if (!handle_heredoc(cmd, tokens))
			return (0);
	}
	return (1);
}

static t_token	*fill_command(t_cmd *cmd, t_token *tokens)
{
	int	arg_count;
	int	arg_index;

	arg_count = count_args(tokens);
	if (!init_args_array(cmd, arg_count))
		return (NULL);
	arg_index = 0;
	while (tokens && tokens->type != TOKEN_PIPE)
	{
		if (!process_token(cmd, &tokens, &arg_index))
			return (NULL);
		tokens = tokens->next;
	}
	return (tokens);
}

static void	add_command_to_list(t_cmd **commands, t_cmd *new_cmd)
{
	t_cmd	*current;

	if (!*commands)
	{
		*commands = new_cmd;
		return ;
	}
	current = *commands;
	while (current->next)
		current = current->next;
	current->next = new_cmd;
}

t_cmd	*build_commands(t_token *tokens)
{
	t_cmd	*commands;
	t_cmd	*new_cmd;

	commands = NULL;
	while (tokens)
	{
		new_cmd = create_command();
		if (!new_cmd)
		{
			free_commands(commands);
			return (NULL);
		}
		tokens = fill_command(new_cmd, tokens);
		add_command_to_list(&commands, new_cmd);
		if (tokens && tokens->type == TOKEN_PIPE)
			tokens = tokens->next;
	}
	return (commands);
}
