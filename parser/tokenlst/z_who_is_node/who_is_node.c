/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   who_is_node.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 17:11:59 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/02 17:13:58 by hde-barr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_part2.h"

bool is_redir_super(char *input)
{
	if(ft_strlen(input) != 1 && ft_strlen(input) != 2)
		return (false);
	if(ft_strlen(input) == 1)
		if(*input == '<' || *input == '>')
			return (true);
	if(ft_strlen(input) == 2)
		if(ft_strcmp("<<", input) == 0 || ft_strcmp(">>", input) == 0)
			return (true);
	return (false);
}

bool is_word_super(char *input)
{
	return (true);
}

bool is_pipe_super(char *input)
{
	if(ft_strlen(input) != 1)
		return(false);
	if(*input == '|')
		return(true);
	return(false);
}

bool is_cmd_super(char *input, char **env)
{
	return (search_list(input, env));
}

bool is_eof(char *input)
{
	if (!input)
		return (true);
	return (false);
}
