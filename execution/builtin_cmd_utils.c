/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabodaya <yabodaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 00:55:48 by yabodaya          #+#    #+#             */
/*   Updated: 2026/06/02 21:09:32 by yabodaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	digit_overflow(unsigned long n, int d, int sign)
{
	unsigned long	limit;
	unsigned long	cut;

	limit = 9223372036854775807UL + (sign < 0);
	cut = limit / 10;
	if (n > cut)
		return (1);
	if (n == cut && (unsigned long)d > limit % 10)
		return (1);
	return (0);
}

int	is_numeric(const char *s)
{
	int				i;
	int				sign;
	unsigned long	n;

	i = 0;
	sign = 1;
	n = 0;
	if (s[i] == '-')
		sign = -1;
	if (s[i] == '-' || s[i] == '+')
		i++;
	if (!s[i])
		return (0);
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (0);
		if (digit_overflow(n, s[i] - '0', sign))
			return (0);
		n = n * 10 + (s[i] - '0');
		i++;
	}
	return (1);
}

long	ft_atol(const char *str)
{
	long	result;
	int		sign;
	int		i;

	result = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] && ft_isdigit(str[i]))
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

int	env_size(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
		i++;
	return (i);
}

void	free_array(char ***arr)
{
	int	i;

	if (!arr || !*arr)
		return ;
	i = 0;
	while ((*arr)[i])
	{
		free((*arr)[i]);
		i++;
	}
	free(*arr);
	*arr = NULL;
}
