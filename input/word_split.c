#include "../includes/minishell.h"

int	count_word_pieces(t_token *token)
{
	char	*v;
	int		i;
	int		pieces;

	v = token->value;
	if (token->quote_type != QUOTE_NONE || !token->expanded)
		return (1);
	i = 0;
	pieces = 0;
	while (v[i])
	{
		while (v[i] && is_whitespace(v[i]))
			i++;
		if (v[i] && !is_whitespace(v[i]))
		{
			pieces++;
			while (v[i] && !is_whitespace(v[i]))
				i++;
		}
	}
	return (pieces);
}

static int	add_piece(t_cmd *cmd, char *start, int len, int *idx)
{
	char	*piece;

	piece = malloc(sizeof(char) * (len + 1));
	if (!piece)
		return (0);
	ft_strlcpy(piece, start, len + 1);
	cmd->args[*idx] = piece;
	(*idx)++;
	return (1);
}

int	add_split_word(t_cmd *cmd, t_token *token, int *idx)
{
	char	*v;
	int		start;
	int		i;

	if (token->quote_type != QUOTE_NONE || !token->expanded)
		return (add_arg_to_command(cmd, token->value, idx));
	v = token->value;
	i = 0;
	while (v[i])
	{
		while (v[i] && is_whitespace(v[i]))
			i++;
		start = i;
		while (v[i] && !is_whitespace(v[i]))
			i++;
		if (i > start)
		{
			if (!add_piece(cmd, v + start, i - start, idx))
				return (0);
		}
	}
	return (1);
}
