/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 19:22:32 by hde-barr          #+#    #+#             */
/*   Updated: 2024/12/10 15:58:29 by hde-barr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"
#include <stdarg.h>

static void	print_percents(const char *str_printf, va_list args, int *n_i)
{
	if (*str_printf == 'c')
		ft_putchar((char)va_arg(args, int), n_i);
	else if (*str_printf == 's')
		ft_putstr(va_arg(args, char *), n_i);
	else if (*str_printf == 'p')
		ft_putptr(va_arg(args, void *), n_i);
	else if (*str_printf == 'i')
		ft_putnbr(va_arg(args, int), n_i);
	else if (*str_printf == 'd')
		ft_putnbr(va_arg(args, int), n_i);
	else if (*str_printf == 'u')
		ft_putunbr_base(va_arg(args, unsigned int), "0123456789", n_i);
	else if (*str_printf == 'x')
		ft_putunbr_base(va_arg(args, unsigned int), "0123456789abcdef", n_i);
	else if (*str_printf == 'X')
		ft_putunbr_base(va_arg(args, unsigned int), "0123456789ABCDEF", n_i);
	else if (*str_printf == '%')
		ft_putchar('%', n_i);
}

int	ft_printf(const char *str_printf, ...)
{
	va_list	args;
	int		*n_i;
	int		i;

	n_i = &i;
	va_start(args, str_printf);
	i = 0;
	while (*str_printf)
	{
		if (*str_printf == '%' && *(str_printf + 1))
			print_percents(++str_printf, args, n_i);
		else
			ft_putchar(*str_printf, n_i);
		str_printf++;
	}
	va_end(args);
	return (i);
}
