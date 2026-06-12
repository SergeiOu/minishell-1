#include "../includes/minishell.h"

static char	*expand_at_index(char *res, int *i, char **envp, int est)
{
	int		var_len;
	char	*var_name;
	char	*var_value;
	char	*temp;

	var_len = get_var_name_len(res + *i + 1);
	if (var_len == 0)
	{
		(*i)++;
		return (res);
	}
	var_name = extract_var_name(res + *i + 1, var_len);
	var_value = get_var_value(var_name, envp, est);
	temp = build_expanded(res, i, var_value, var_len);
	free(res);
	free(var_name);
	free(var_value);
	return (temp);
}

char	*expand_value(char *value, char **envp, int in_squote, int est)
{
	char	*result;
	int		i;

	if (in_squote || !should_expand(value))
		return (ft_strdup(value));
	result = ft_strdup(value);
	if (!result)
		return (NULL);
	i = 0;
	while (result && result[i])
	{
		if (result[i] == '$' && result[i + 1])
			result = expand_at_index(result, &i, envp, est);
		else
			i++;
	}
	return (result);
}

void	expand_tokens(t_token *tokens, char **envp, int exit_status)
{
	t_token	*current;
	char	*expanded;

	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_WORD)
		{
			if (current->quote_type == QUOTE_NONE
				&& should_expand(current->value))
				current->expanded = 1;
			expanded = expand_value(current->value,
					envp, (current->quote_type == QUOTE_SINGLE),
					exit_status);
			if (!expanded)
				expanded = ft_strdup("");
			free(current->value);
			current->value = expanded;
		}
		current = current->next;
	}
}
