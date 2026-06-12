/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sounania <sounania@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 10:38:31 by sounania          #+#    #+#             */
/*   Updated: 2026/02/02 12:13:41 by sounania         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_pipe(t_token *token)
{
	return (token && token->type == TOKEN_PIPE);
}

int	validate_pipes(t_token *tokens)
{
	t_token	*current;

	if (!tokens)
		return (1);
	if (is_pipe(tokens))
		return (0);
	current = tokens;
	while (current)
	{
		if (is_pipe(current))
		{
			if (!current->next || is_pipe(current->next))
				return (0);
		}
		current = current->next;
	}
	return (1);
}

static int	is_redir(t_token *token)
{
	if (!token)
		return (0);
	return (token->type == TOKEN_REDIR_IN
		|| token->type == TOKEN_REDIR_OUT
		|| token->type == TOKEN_REDIR_APPEND
		|| token->type == TOKEN_REDIR_HEREDOC);
}

int	validate_redirections(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (is_redir(current))
		{
			if (!current->next)
				return (0);
			if (current->next->type != TOKEN_WORD)
				return (0);
		}
		current = current->next;
	}
	return (1);
}
