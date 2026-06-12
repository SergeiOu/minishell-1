/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabodaya <yabodaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 15:33:06 by yabodaya          #+#    #+#             */
/*   Updated: 2025/06/16 12:06:04 by yabodaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*temp;

	if (nmemb != 0 && (nmemb * size) / nmemb != size)
		return (NULL);
	temp = malloc(nmemb * size);
	if (temp == NULL)
		return (NULL);
	ft_memset(temp, 0, nmemb * size);
	return (temp);
}
