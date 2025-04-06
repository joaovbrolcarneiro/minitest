/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 11:54:32 by hde-barr          #+#    #+#             */
/*   Updated: 2024/10/24 11:41:25 by hde-barr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const char	*ps1;
	const char	*ps2;

	ps1 = s1;
	ps2 = s2;
	while (n > 0)
	{
		if ((unsigned char)*ps1 != (unsigned char)*ps2)
			return ((unsigned char)*ps1 - (unsigned char)*ps2);
		ps1++;
		ps2++;
		n--;
	}
	return (0);
}
