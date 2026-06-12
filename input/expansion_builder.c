/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_builder.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabodaya <yabodaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 11:27:51 by yabodaya          #+#    #+#             */
/*   Updated: 2026/06/02 21:09:32 by yabodaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	copy_parts(const char *s, char *result, int *i,
		const char *vv)
{
	int	j;
	int	k;
	int	vnl;

	vnl = get_var_name_len(s + *i + 1);
	j = 0;
	k = 0;
	while (k < *i)
		result[j++] = s[k++];
	k = 0;
	while (vv && vv[k])
		result[j++] = vv[k++];
	k = *i + 1 + vnl;
	while (s[k])
		result[j++] = s[k++];
	result[j] = '\0';
	*i = *i + ft_strlen(vv);
}

char	*build_expanded(const char *s, int *i, const char *vv, int vnl)
{
	char	*result;
	int		rlen;

	rlen = *i + ft_strlen(vv) + ft_strlen(s + *i + 1 + vnl);
	result = malloc(rlen + 1);
	if (!result)
		return (NULL);
	copy_parts(s, result, i, vv);
	return (result);
}
