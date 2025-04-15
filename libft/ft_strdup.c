/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrol-ca <jbrol-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 11:55:55 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/15 18:17:21 by jbrol-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_strcpy(char *dest, const char *src)
{
	size_t	i;

	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
}

char	*ft_strdup(const char *s)
{
	char	*cpy;

	cpy = (char *)hb_malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!cpy)
		return (cpy);
	ft_strcpy(cpy, s);
	return (cpy);
}
