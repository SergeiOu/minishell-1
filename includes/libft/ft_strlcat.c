/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabodaya <yabodaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 10:27:36 by yabodaya          #+#    #+#             */
/*   Updated: 2025/06/16 11:41:42 by yabodaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	dstn;
	size_t	srclen;
	size_t	o_dest_val;

	srclen = ft_strlen(src);
	dstn = ft_strlen(dst);
	o_dest_val = dstn;
	if (size <= dstn)
		return (srclen + size);
	i = 0;
	while (dstn < size - 1 && src[i] != '\0')
	{
		dst[dstn] = src[i];
		dstn++;
		i++;
	}
	dst[dstn] = '\0';
	return (o_dest_val + srclen);
}
