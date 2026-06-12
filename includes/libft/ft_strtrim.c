/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabodaya <yabodaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 08:38:32 by yabodaya          #+#    #+#             */
/*   Updated: 2025/06/16 12:41:51 by yabodaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	size_t	size;
	size_t	i;
	char	*c;

	start = 0;
	while (s1[start] && ft_strchr(set, s1[start]))
		start ++;
	end = ft_strlen(s1);
	while (end > start && ft_strchr(set, s1[end - 1]))
		end--;
	size = end - start;
	c = malloc(size + 1);
	if (c == NULL)
		return (NULL);
	i = 0;
	while (i < size)
	{
		c[i] = s1[start + i];
		i++;
	}
	c[i] = '\0';
	return (c);
}
