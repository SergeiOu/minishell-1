#include "../includes/minishell.h"

static void	join_into_prev(t_token *prev, t_token *cur)
{
	char	*merged;

	merged = ft_strjoin(prev->value, cur->value);
	free(prev->value);
	prev->value = merged;
	if (cur->has_quoted)
		prev->has_quoted = 1;
	prev->next = cur->next;
	free(cur->value);
	free(cur);
}

void	merge_joined_tokens(t_token **tokens)
{
	t_token	*cur;

	if (!tokens || !*tokens)
		return ;
	cur = *tokens;
	while (cur && cur->next)
	{
		if (cur->next->type == TOKEN_WORD && cur->next->joined
			&& cur->type == TOKEN_WORD)
			join_into_prev(cur, cur->next);
		else
			cur = cur->next;
	}
}
