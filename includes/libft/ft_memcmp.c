/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabodaya <yabodaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 14:28:14 by yabodaya          #+#    #+#             */
/*   Updated: 2025/06/16 10:50:10 by yabodaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		if (*((const unsigned char *)s1 + i) != *(
				(const unsigned char *)s2 + i))
		{
			return (*((const unsigned char *)s1 + i) - *(
					(const unsigned char *)s2 + i));
		}
		i++;
	}
	return (0);
}
