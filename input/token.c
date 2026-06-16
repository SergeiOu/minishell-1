#include "../includes/minishell.h"

t_token	*create_token(t_token_type type, char *value,
		t_quote_type quote_type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = ft_strdup(value);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	token->quote_type = quote_type;
	token->joined = 0;
	token->expanded = 0;
	token->has_quoted = (quote_type != QUOTE_NONE);
	token->next = NULL;
	return (token);
}

void	add_token_to_list(t_token **tokens, t_token_type type,
		char *value, t_quote_type quote_type)
{
	t_token	*new_token;
	t_token	*current;

	new_token = create_token(type, value, quote_type);
	if (!new_token)
		return ;
	if (!*tokens)
	{
		*tokens = new_token;
		return ;
	}
	current = *tokens;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

void	free_tokens(t_token *tokens)
{
	t_token	*temp;

	while (tokens)
	{
		temp = tokens;
		tokens = tokens->next;
		free(temp->value);
		free(temp);
	}
}

static void	mark_last_joined(t_token *tokens)
{
	t_token	*last;

	if (!tokens)
		return ;
	last = tokens;
	while (last->next)
		last = last->next;
	if (last->type == TOKEN_WORD)
		last->joined = 1;
}

t_token	*tokenize(char *input)
{
	t_token	*tokens;
	int		i;
	int		end_prev;
	int		token_start;

	tokens = NULL;
	i = 0;
	end_prev = -1;
	while (input[i])
	{
		skip_spaces(input, &i);
		if (!input[i])
			break ;
		token_start = i;
		if (!handle_operator(input, &i, &tokens))
			break ;
		if (token_start == end_prev)
			mark_last_joined(tokens);
		end_prev = i;
	}
	return (tokens);
}
