/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 13:56:51 by hde-barr          #+#    #+#             */
/*   Updated: 2024/10/24 09:50:29 by hde-barr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;
	size_t	little_len;

	i = 0;
	little_len = ft_strlen(little);
	if (!(*little))
		return ((char *)big);
	while (big[i] && len)
	{
		j = 0;
		while (big[i + j] == little[j] && big[i + j] && little_len <= len)
			j++;
		if (!little[j])
			return ((char *)&big[i]);
		i++;
		len--;
	}
	return (NULL);
}
