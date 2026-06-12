/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 11:26:48 by sounania          #+#    #+#             */
/*   Updated: 2026/02/17 19:59:46 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	count_args(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens && tokens->type != TOKEN_PIPE)
	{
		if (tokens->type == TOKEN_WORD)
			count += count_word_pieces(tokens);
		else if (tokens->type == TOKEN_REDIR_IN
			|| tokens->type == TOKEN_REDIR_OUT
			|| tokens->type == TOKEN_REDIR_APPEND
			|| tokens->type == TOKEN_REDIR_HEREDOC)
		{
			tokens = tokens->next;
			if (tokens)
				tokens = tokens->next;
			continue ;
		}
		tokens = tokens->next;
	}
	return (count);
}

int	add_arg_to_command(t_cmd *cmd, char *arg, int *arg_index)
{
	if (!cmd->args)
		return (0);
	cmd->args[*arg_index] = ft_strdup(arg);
	if (!cmd->args[*arg_index])
		return (0);
	(*arg_index)++;
	return (1);
}

int	init_args_array(t_cmd *cmd, int arg_count)
{
	cmd->args = malloc(sizeof(char *) * (arg_count + 1));
	if (!cmd->args)
		return (0);
	cmd->args[arg_count] = NULL;
	return (1);
}
