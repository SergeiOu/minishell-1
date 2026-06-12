/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabodaya <yabodaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 14:24:22 by yabodaya          #+#    #+#             */
/*   Updated: 2025/06/16 12:55:09 by yabodaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	getsize(long n)
{
	int	s;

	s = 0;
	if (n == 0)
		return (1);
	if (n < 0)
		n = -n;
	while (n != 0)
	{
		n /= 10;
		s++;
	}
	return (s);
}

static char	*reverse(char *c)
{
	int		i;
	int		j;
	char	tmp;

	i = 0;
	j = ft_strlen(c) - 1;
	while (i < j)
	{
		tmp = c[i];
		c[i] = c[j];
		c[j] = tmp;
		i++;
		j--;
	}
	return (c);
}

char	*ft_itoa(int n)
{
	char	*c;
	int		i;
	int		negative;
	long	nb;

	nb = n;
	negative = (nb < 0);
	if (nb < 0)
		nb = -nb;
	i = 0;
	c = malloc(getsize(n) + negative + 1);
	if (!c)
		return (NULL);
	if (nb == 0)
		c[i++] = '0';
	while (nb != 0)
	{
		c[i++] = (nb % 10) + '0';
		nb /= 10;
	}
	if (negative)
		c[i++] = '-';
	c[i] = '\0';
	return (reverse(c));
}
