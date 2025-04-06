/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 14:04:15 by hde-barr          #+#    #+#             */
/*   Updated: 2024/11/30 17:27:45 by hde-barr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

void	*ft_calloc_gnl(size_t nmemb, size_t size)
{
	size_t			i;
	unsigned char	*str;

	str = malloc(nmemb * size);
	if (!str)
		return (NULL);
	i = 0;
	while (i < (nmemb * size))
		str[i++] = '\0';
	return (str);
}

char	*ft_strchr_gnl(char *s, int c)
{
	if (!s)
		return (NULL);
	while (*s != c)
	{
		if (*s == '\0')
			return (NULL);
		s++;
	}
	return ((char *) s);
}

int	strlen_gnl(const char *str)
{
	int	len;

	if (!str)
		return (0);
	len = 0;
	while (str[len])
		len++;
	return (len);
}

char	*ft_strjoin_gnl(char *s1, char *s2)
{
	int		i;
	int		j;
	char	*str;

	if (!s1)
		s1 = (char *)ft_calloc_gnl(1, sizeof(char));
	if (!s1 || !s2)
		return (NULL);
	if (strlen_gnl(s1) + strlen_gnl(s2) == 0)
		return (free(s1), NULL);
	str = ft_calloc_gnl((strlen_gnl(s1) + strlen_gnl(s2) + 1), sizeof(char));
	if (!str)
		return (free(s1), NULL);
	i = -1;
	while (s1[++i])
		str[i] = s1[i];
	j = 0;
	while (s2[j])
		str[i++] = s2[j++];
	return (free(s1), str);
}

char	*ft_rebuffer(char *buffer)
{
	int		i;
	int		j;
	char	*cache;

	i = 0;
	while (buffer[i] != '\n' && buffer[i])
		i++;
	if (!buffer[i])
		return (free(buffer), NULL);
	cache = ft_calloc_gnl((strlen_gnl(buffer) - i + 1), sizeof(char));
	if (!cache)
		return (NULL);
	j = 0;
	while (buffer[i++])
		cache[j++] = buffer[i];
	cache[j] = '\0';
	return (free (buffer), cache);
}
