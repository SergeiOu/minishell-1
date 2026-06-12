/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabodaya <yabodaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 11:27:24 by yabodaya          #+#    #+#             */
/*   Updated: 2026/06/02 21:09:32 by yabodaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	should_expand(const char *token_value)
{
	int	i;

	i = 0;
	while (token_value[i])
	{
		if (token_value[i] == '$' && token_value[i + 1])
			return (1);
		i++;
	}
	return (0);
}
