/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabodaya <yabodaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 09:37:41 by yabodaya          #+#    #+#             */
/*   Updated: 2025/06/16 12:53:00 by yabodaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(const char *s, char c)
{
	int	count;
	int	word;

	count = 0;
	word = 0;
	while (*s)
	{
		if (*s != c && !word)
		{
			count++;
			word = 1;
		}
		if (*s == c)
			word = 0;
		s++;
	}
	return (count);
}

static int	ft_word_len(const char *s, char c)
{
	int	len;

	len = 0;
	while (s[len] && s[len] != c)
		len++;
	return (len);
}

static char	*ft_word_dup(const char *s, char c)
{
	char	*word;
	int		len;
	int		i;

	len = ft_word_len(s, c);
	word = (char *)malloc((len + 1) * sizeof(char));
	if (!word)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = s[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

static void	ft_free_all(char **arr, int size)
{
	while (size > 0)
	{
		size--;
		free(arr[size]);
	}
	free(arr);
}

char	**ft_split(char const *s, char c)
{
	char	**result;
	int		i;
	int		word_count;

	if (!s)
		return (NULL);
	word_count = count_words(s, c);
	result = (char **)malloc((word_count + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	i = 0;
	while (*s && i < word_count)
	{
		while (*s == c)
			s++;
		result[i] = ft_word_dup(s, c);
		if (!result[i++])
		{
			ft_free_all(result, i);
			return (NULL);
		}
		s += ft_word_len(s, c);
	}
	result[i] = NULL;
	return (result);
}
