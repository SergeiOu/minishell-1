/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabodaya <yabodaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 15:25:43 by yabodaya          #+#    #+#             */
/*   Updated: 2025/06/16 11:48:15 by yabodaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	if (little[0] == '\0')
	{
		return ((char *)big);
	}
	i = 0;
	while (big[i] && i < len)
	{
		j = 0;
		while (little[j] && (big[i + j] == little[j]) && (i + j) < len)
			j++;
		if (!little[j])
		{
			return ((char *)&big[i]);
		}
		i++;
	}
	return (NULL);
}
