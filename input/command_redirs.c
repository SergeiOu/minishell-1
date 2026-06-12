/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_redirs.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabodaya <yabodaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 11:27:54 by yabodaya          #+#    #+#             */
/*   Updated: 2026/06/02 21:09:32 by yabodaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_redir_in(t_cmd *cmd, t_token **current)
{
	char	*target;

	*current = (*current)->next;
	if (!*current || (*current)->type != TOKEN_WORD)
		return (0);
	target = ft_strdup((*current)->value);
	if (!target)
		return (0);
	if (!add_redir(cmd, REDIR_IN, target, 0))
		return (free(target), 0);
	return (1);
}

int	handle_redir_out(t_cmd *cmd, t_token **current)
{
	int		is_append;
	char	*target;

	is_append = ((*current)->type == TOKEN_REDIR_APPEND);
	*current = (*current)->next;
	if (!*current || (*current)->type != TOKEN_WORD)
		return (0);
	target = ft_strdup((*current)->value);
	if (!target)
		return (0);
	if (is_append)
		is_append = REDIR_APPEND;
	else
		is_append = REDIR_OUT;
	if (!add_redir(cmd, is_append, target, 0))
		return (free(target), 0);
	return (1);
}

int	handle_heredoc(t_cmd *cmd, t_token **current)
{
	int		quoted;
	char	*target;

	*current = (*current)->next;
	if (!*current || (*current)->type != TOKEN_WORD)
		return (0);
	quoted = ((*current)->quote_type != QUOTE_NONE);
	target = ft_strdup((*current)->value);
	if (!target)
		return (0);
	if (!add_redir(cmd, REDIR_HEREDOC, target, quoted))
		return (free(target), 0);
	return (1);
}
