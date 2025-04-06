/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 07:26:02 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/03 17:54:57 by hde-barr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_nchars(int n)
{
	int	count;

	count = 0;
	if (n == 0)
		return (1);
	if (n < 0)
		count++;
	while (n)
	{
		n /= 10;
		count++;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	int		nchars;
	char	*str;
	long	nb;

	nchars = ft_nchars(n);
	str = (char *)hb_malloc(sizeof(char) * (1 + nchars));
	if (!str)
		return (NULL);
	nb = n;
	str[nchars] = 0;
	if (n == 0)
		str[0] = '0';
	if (nb < 0)
	{
		str[0] = '-';
		nb = -nb;
	}
	while (nb)
	{
		str[--nchars] = nb % 10 + '0';
		nb /= 10;
	}
	return (str);
}
