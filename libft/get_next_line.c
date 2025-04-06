/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 14:02:30 by hde-barr          #+#    #+#             */
/*   Updated: 2024/11/30 17:32:17 by hde-barr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_take_line(char *buffer)
{
	int		i;
	char	*line;

	i = 0;
	if (!buffer[i])
		return (NULL);
	while (buffer[i] != '\n' && buffer[i])
		i++;
	line = ft_calloc_gnl((i + 2), sizeof(char));
	if (!line)
		return (NULL);
	i = 0;
	while (buffer[i] != '\n' && buffer[i])
	{
		line[i] = buffer[i];
		i++;
	}
	if (buffer[i] == '\n')
	{
		line[i] = buffer[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

char	*ft_take_date(int fd, char *buffer)
{
	char	*cache;
	int		cache_len;

	cache = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!cache && buffer)
		return (free(buffer), NULL);
	else if (!cache)
		return (NULL);
	cache_len = 1;
	while (!ft_strchr_gnl(buffer, '\n') && cache_len > 0)
	{
		cache_len = read(fd, cache, BUFFER_SIZE);
		if (cache_len < 0)
			return (free(cache), free(buffer), NULL);
		cache[cache_len] = '\0';
		buffer = ft_strjoin_gnl(buffer, cache);
		if (!buffer)
			return (free(cache), NULL);
	}
	return (free(cache), buffer);
}

char	*get_next_line(int fd)
{
	char			*line;
	static char		*buffer;

	if (fd < 0 || BUFFER_SIZE < 1)
		return (NULL);
	buffer = ft_take_date(fd, buffer);
	if (!buffer)
		return (NULL);
	line = ft_take_line(buffer);
	buffer = ft_rebuffer(buffer);
	return (line);
}
