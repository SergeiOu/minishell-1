/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling_tokenization.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabodaya <yabodaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 10:39:36 by sounania          #+#    #+#             */
/*   Updated: 2026/02/06 14:39:18 by yabodaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_double_operator(char *input, int *i, t_token **tokens)
{
	if (input[*i] == '<' && input[*i + 1] == '<')
	{
		add_token_to_list(tokens, TOKEN_REDIR_HEREDOC, "<<", QUOTE_NONE);
		(*i) += 2;
		return (1);
	}
	if (input[*i] == '>' && input[*i + 1] == '>')
	{
		add_token_to_list(tokens, TOKEN_REDIR_APPEND, ">>", QUOTE_NONE);
		(*i) += 2;
		return (1);
	}
	return (0);
}

int	handle_single_operator(char *input, int *i, t_token **tokens)
{
	if (input[*i] == '|')
		add_token_to_list(tokens, TOKEN_PIPE, "|", QUOTE_NONE);
	else if (input[*i] == '<')
		add_token_to_list(tokens, TOKEN_REDIR_IN, "<", QUOTE_NONE);
	else if (input[*i] == '>')
		add_token_to_list(tokens, TOKEN_REDIR_OUT, ">", QUOTE_NONE);
	else
		return (0);
	(*i)++;
	return (1);
}

int	handle_quote(char *input, int *i, t_token **tokens)
{
	char			quote_char;
	char			*word;
	t_quote_type	quote_type;

	if (!is_quote(input[*i]))
		return (0);
	quote_char = input[*i];
	word = extract_quoted(input, i, quote_char);
	if (word)
	{
		if (quote_char == '\'')
			quote_type = QUOTE_SINGLE;
		else
			quote_type = QUOTE_DOUBLE;
		add_token_to_list(tokens, TOKEN_WORD, word, quote_type);
		free(word);
	}
	return (1);
}

int	handle_word(char *input, int *i, t_token **tokens)
{
	char			*word;
	t_quote_type	quote_type;

	word = extract_word(input, i, &quote_type);
	if (word)
	{
		add_token_to_list(tokens, TOKEN_WORD, word, quote_type);
		free(word);
		return (1);
	}
	return (0);
}

int	handle_operator(char *input, int *i, t_token **tokens)
{
	if (handle_quote(input, i, tokens))
		return (1);
	if (handle_double_operator(input, i, tokens))
		return (1);
	if (handle_single_operator(input, i, tokens))
		return (1);
	if (handle_word(input, i, tokens))
		return (1);
	return (0);
}
