#include "../includes/minishell.h"

int	count_word_pieces(t_token *token)
{
	char	*v;
	int		i;
	int		pieces;

	v = token->value;
	i = 0;
	pieces = 0;
	while (v[i])
	{
		while (v[i] && v[i] == SPLIT_MARK)
			i++;
		if (v[i] && v[i] != SPLIT_MARK)
		{
			pieces++;
			while (v[i] && v[i] != SPLIT_MARK)
				i++;
		}
	}
	if (pieces == 0 && token->has_quoted)
		return (1);
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
	int		found;

	v = token->value;
	i = 0;
	found = 0;
	while (v[i])
	{
		while (v[i] && v[i] == SPLIT_MARK)
			i++;
		start = i;
		while (v[i] && v[i] != SPLIT_MARK)
			i++;
		if (i > start && add_piece(cmd, v + start, i - start, idx))
			found = 1;
		else if (i > start)
			return (0);
	}
	if (!found && token->has_quoted)
		return (add_arg_to_command(cmd, "", idx));
	return (1);
}
