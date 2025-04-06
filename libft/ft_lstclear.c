/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 07:34:35 by hde-barr          #+#    #+#             */
/*   Updated: 2024/10/24 09:41:24 by hde-barr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*cache_next;

	while (*lst)
	{
		cache_next = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = cache_next;
	}
	*lst = NULL;
}
