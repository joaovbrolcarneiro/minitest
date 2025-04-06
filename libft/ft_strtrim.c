/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 07:24:21 by hde-barr          #+#    #+#             */
/*   Updated: 2024/10/24 11:19:21 by hde-barr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_ischarset(const char *set, char c)
{
	while (*set)
	{
		if (*set == c)
			return (1);
		set++;
	}
	return (0);
}

char	*ft_strtrim(const char *s1, const char *set)
{
	size_t	start;
	size_t	end;
	char	*dst;

	start = 0;
	end = ft_strlen(s1) - 1;
	while (ft_ischarset(set, s1[start]))
		start++;
	while (ft_ischarset(set, s1[end]))
		end--;
	dst = ft_substr(s1, start, (end - start) + 1);
	return (dst);
}
