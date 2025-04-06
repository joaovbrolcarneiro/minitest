/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 07:34:54 by hde-barr          #+#    #+#             */
/*   Updated: 2024/10/24 09:45:20 by hde-barr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	void	*content;
	t_list	*newcontent;
	t_list	*newlst;

	newlst = NULL;
	while (lst)
	{
		content = f(lst->content);
		newcontent = ft_lstnew(content);
		if (!newcontent)
		{
			del(content);
			ft_lstclear(&newlst, del);
			return (NULL);
		}
		else
			ft_lstadd_back(&newlst, newcontent);
		lst = lst->next;
	}
	return (newlst);
}
