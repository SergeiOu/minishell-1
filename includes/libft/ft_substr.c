/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabodaya <yabodaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 17:53:34 by yabodaya          #+#    #+#             */
/*   Updated: 2025/06/16 12:33:14 by yabodaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*empty_string(void)
{
	char	*c;

	c = (char *)malloc(1);
	if (!c)
		return (NULL);
	c[0] = '\0';
	return (c);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*c;
	size_t	str_len;
	size_t	i;

	if (!s)
		return (NULL);
	str_len = ft_strlen(s);
	if (start >= str_len)
		return (empty_string());
	if (len > str_len - start)
		len = str_len - start;
	c = (char *)malloc(len + 1);
	if (!c)
		return (NULL);
	i = 0;
	while (i < len && s[start + i])
	{
		c[i] = s[start + i];
		i++;
	}
	c[i] = '\0';
	return (c);
}
