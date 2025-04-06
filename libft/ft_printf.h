/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 12:07:02 by hde-barr          #+#    #+#             */
/*   Updated: 2024/12/10 15:58:55 by hde-barr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include "libft.h"

int		ft_printf(const char *str_printf, ...);
void	ft_putunbr_base(unsigned int nbr, char *base, int *n_i);
void	ft_putulnbr_base(unsigned long nbr, char *base, int *n_i);
void	ft_putptr(void *ptr, int *n_i);
void	ft_putchar(char c, int *n_i);
void	ft_putstr(char *s, int *n_i);
void	ft_putnbr(int n, int *n_i);
#endif
