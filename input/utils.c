/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sounania <sounania@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 10:40:12 by sounania          #+#    #+#             */
/*   Updated: 2026/02/06 12:05:00 by sounania         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

void	skip_spaces(char *input, int *i)
{
	while (input && *i >= 0 && input[*i] && is_whitespace(input[*i]))
		(*i)++;
}
