/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   z_merge_to_token_exception.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 19:17:54 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/02 19:18:23 by hde-barr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_part2.h"

bool merge_to_token_exception(t_token *token)
{
	t_token *first;
	t_token *rm_node;
	first = token;
	if(token->merge_next == true)
	{
		rm_node = rm_node_lst(token->next, first);
		token->value = ft_strjoin(token->value, rm_node->value);
	}
	return (true);
}