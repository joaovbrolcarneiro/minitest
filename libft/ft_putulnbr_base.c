/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putulnbr_base.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:09:36 by hde-barr          #+#    #+#             */
/*   Updated: 2024/11/06 19:05:27 by hde-barr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static	void	ft_print_nbr(unsigned long nbr, char *base, int *n_i)
{
	unsigned int	base_n;
	char			res;

	base_n = ft_strlen(base);
	res = base[nbr % base_n];
	if (nbr >= base_n)
		ft_print_nbr(nbr / base_n, base, n_i);
	*n_i += write(1, &res, 1);
}

static	int	ft_is_base_ok(char *base)
{
	int	res;
	int	i;
	int	j;

	res = 1;
	i = 0;
	if (ft_strlen(base) <= 1)
		return (res = 0);
	while (base[i])
	{
		if (base[i] == '-' || base[i] == '+')
			return (res = 0);
		j = i + 1;
		while (base[j])
		{
			if (base[i] == base[j])
				return (res = 0);
			j++;
		}
		i++;
	}
	return (res);
}

void	ft_putulnbr_base(unsigned long nbr, char *base, int *n_i)
{
	if (!ft_is_base_ok(base))
		return ;
	ft_print_nbr(nbr, base, n_i);
}
