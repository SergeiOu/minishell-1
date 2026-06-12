#include "../includes/minishell.h"

int	has_unclosed_quotes(char *input)
{
	int		i;
	char	quote;

	i = 0;
	while (input[i])
	{
		if (is_quote(input[i]))
		{
			quote = input[i];
			i++;
			while (input[i] && input[i] != quote)
				i++;
			if (!input[i])
				return (1);
		}
		i++;
	}
	return (0);
}

int	validate_syntax(char *input, t_token *tokens)
{
	if (!input || !*input)
		return (1);
	if (has_unclosed_quotes(input))
	{
		ft_putstr_fd("minishell: syntax error: unclosed quotes\n", 2);
		return (0);
	}
	if (!validate_pipes(tokens))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		return (0);
	}
	if (!validate_redirections(tokens))
	{
		ft_putstr_fd("minishell: syntax error near unexpected", 2);
		ft_putstr_fd(" token `newline'\n", 2);
		return (0);
	}
	return (1);
}
