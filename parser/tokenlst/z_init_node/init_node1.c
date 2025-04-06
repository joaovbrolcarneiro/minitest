/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_node1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 16:49:49 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/02 16:52:07 by hde-barr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_part2.h"

t_token *new_pipe(t_token *token)
{
	token->used = false;
	token->rank = RANK_S;
	token->id = get_token_id();
	token->type = TOKEN_PIPE;
	token->value = token->value;
	token->args = NULL;
	token->file = NULL;
	token->coretype = TOKEN_PIPE;
	if(token->literal == true)
	{
		token->type = TOKEN_WORD;
		token->rank = RANK_C;
		token->coretype = TOKEN_WORD;
	}
	//token->next = NULL;
	return (token);
}

t_token *new_redir(t_token *token)
{
	token->used = false;
	token->rank = RANK_S;
	token->id = get_token_id();
	if(ft_strcmp("<<", token->value) == 0)
		token->type = TOKEN_HEREDOC;
	else if(ft_strcmp(">>", token->value) == 0)
		token->type = TOKEN_APPEND;
	else if(*token->value == '<')
		token->type = TOKEN_REDIR_IN;
	else if(*token->value == '>')
		token->type = TOKEN_REDIR_OUT;
	token->value = token->value;
	token->args = NULL;
	token->file = NULL;
	token->coretype = REDIR;
	if(token->literal == true)
	{
		token->type = TOKEN_WORD;
		token->rank = RANK_C;
		token->coretype = TOKEN_WORD;
	}
	//token->next = NULL;
	return (token);
}

t_token *new_cmd(t_token *token)
{
	token->used = false;
	token->rank = RANK_B;
	token->id = get_token_id();
	token->type = TOKEN_CMD;
	token->value = token->value;
	token->args = NULL;
	token->file = NULL;
	token->coretype = TOKEN_CMD;
	//token->next = NULL;
	return (token);
}

t_token *new_word(t_token *token)
{
	token->used = false;
	token->rank = RANK_C;
	token->id = get_token_id();
	token->type = TOKEN_WORD;
	token->value = token->value;
	token->args = NULL;
	token->file = NULL;
	token->coretype = TOKEN_WORD;
	//token->next = NULL;
	return (token);
}

t_token *new_eof(t_token *token)
{
	token->id = get_token_id();
	token->next = NULL;
	token->used = false;
	token->rank = RANK_SS;
	token->type = TOKEN_EOF;
	token->args = NULL;
	token->value = NULL;
	token->file = NULL;
	token->coretype = TOKEN_EOF;
	return (token);
}
