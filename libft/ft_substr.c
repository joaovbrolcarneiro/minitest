/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 15:34:11 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/03 17:56:30 by hde-barr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	char	*sub_malloc;

	if ((ft_strlen(s) <= start) || !(*s))
		return (ft_strdup(""));
	sub = ft_strnstr(s, &s[start], ft_strlen(s));
	if (len > ft_strlen(sub))
		len = ft_strlen(sub);
	sub_malloc = (char *)hb_malloc(sizeof(char) * (len + 1));
	if (!sub_malloc)
		return (NULL);
	ft_bzero((char *)sub_malloc, len + 1);
	ft_strlcpy(sub_malloc, sub, len + 1);
	return (sub_malloc);
}
