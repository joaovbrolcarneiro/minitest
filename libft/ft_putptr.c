/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putptr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 13:34:49 by hde-barr          #+#    #+#             */
/*   Updated: 2024/11/06 19:01:39 by hde-barr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_putptr(void *ptr, int *n_i)
{
	if ((unsigned long)ptr == 0)
	{
		*n_i += write(1, "(nil)", 5);
		return ;
	}
	ft_putstr("0x", n_i);
	ft_putulnbr_base((unsigned long)ptr, "0123456789abcdef", n_i);
}
