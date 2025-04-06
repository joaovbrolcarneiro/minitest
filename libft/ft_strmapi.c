/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 07:27:30 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/03 17:56:15 by hde-barr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*dst;
	unsigned int	s_len;
	unsigned int	index;

	index = 0;
	s_len = ft_strlen(s);
	dst = (char *)hb_malloc(sizeof(char) * (s_len + 1));
	if (!dst)
		return (NULL);
	ft_bzero(dst, s_len + 1);
	while (s[index])
	{
		dst[index] = f(index, s[index]);
		index++;
	}
	return (dst);
}
