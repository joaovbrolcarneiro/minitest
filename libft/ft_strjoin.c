/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 07:24:10 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/03 17:56:08 by hde-barr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*dst;
	size_t	dst_len;

	dst_len = ft_strlen(s1) + ft_strlen(s2);
	dst = (char *)hb_malloc(sizeof(char) * (dst_len + 1));
	if (!dst)
		return (NULL);
	ft_bzero((char *)dst, dst_len + 1);
	ft_strlcpy(dst, s1, dst_len + 1);
	ft_strlcat(dst, s2, dst_len + 1);
	return (dst);
}
