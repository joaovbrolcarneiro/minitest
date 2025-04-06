/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 07:24:29 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/03 17:55:53 by hde-barr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_countwords(char *s, char c)
{
	size_t	i;

	i = 0;
	while (*s)
	{
		while (*s == c && *s)
			s++;
		if (*s)
			i++;
		while (*s != c && *s)
			s++;
	}
	return (i);
}

static int	ft_wordlen(char *str, char end)
{
	size_t	len;

	len = 0;
	while (str[len] && str[len] != end)
	{
		len++;
	}
	return (len);
}

static char	*ft_takeword(char *s, char gap, size_t index)
{
	size_t	i;
	size_t	nowindex;
	char	*word;

	nowindex = 0;
	i = 0;
	while (nowindex <= index)
	{
		s += i;
		while (*s == gap)
			s++;
		if (*s)
			i = ft_wordlen(s, gap);
		nowindex++;
	}
	word = (char *)hb_malloc(sizeof(char) * (i + 1));
	if (!word)
		return (NULL);
	ft_bzero(word, i + 1);
	ft_strlcpy(word, s, i + 1);
	return (word);
}

static void	*ft_free(char **matrix, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		free(matrix[i]);
		i++;
	}
	free(matrix);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	size_t	nwords;
	size_t	i;
	char	**matrix;

	i = 0;
	nwords = ft_countwords((char *)s, c);
	matrix = (char **)hb_malloc(sizeof(char *) * (nwords + 1));
	if (!matrix)
		return (NULL);
	while (i < nwords)
	{
		matrix[i] = ft_takeword((char *)s, c, i);
		if (!matrix[i])
			return (ft_free(matrix, i));
		i++;
	}
	matrix[i] = NULL;
	return (matrix);
}
