/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 11:54:48 by hde-barr          #+#    #+#             */
/*   Updated: 2024/10/24 08:46:25 by hde-barr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char		*pdst;
	const char	*psrc;

	if (dest == src)
		return (dest);
	pdst = dest;
	psrc = src;
	if (dest < src)
	{
		while (n--)
			*pdst++ = *psrc++;
	}
	else
	{
		pdst += n - 1;
		psrc += n - 1;
		while (n--)
			*pdst-- = *psrc--;
	}
	return (dest);
}
