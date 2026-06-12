/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabodaya <yabodaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 18:15:19 by yabodaya          #+#    #+#             */
/*   Updated: 2025/06/10 12:20:25 by yabodaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	x;
	size_t	i;
	char	*c;

	x = ft_strlen(s1) + ft_strlen(s2);
	c = (char *)malloc(x + 1);
	if (c == NULL)
		return (NULL);
	x = 0;
	while (s1[x])
	{
		c[x] = s1[x];
		x++;
	}
	i = 0;
	while (s2[i])
	{
		c[x] = s2[i];
		x++;
		i++;
	}
	c[x] = '\0';
	return (c);
}
