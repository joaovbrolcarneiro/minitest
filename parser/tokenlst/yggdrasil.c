/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   yggdrasil.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 21:06:50 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/02 17:29:11 by hde-barr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "minishell.h"
#include "minishell_part2.h"

t_token *new_eof(t_token *token);

t_token *find_right_token(t_token *token, t_token *eof)////////yggdrasil
{
	t_token *father;
	int i;

	i = 0;
	if(!token)
		return(NULL);
	if(token->next == eof || !token->next || token == eof)
		return (NULL);
	father = token;
	token = token->next;
	while (1)
	{
		if(father->rank - i == token->rank)
			return (token);
		if (father->rank - i == RANK_F)
			return (NULL);
		token = token->next;
		if(token == eof)
		{
			i++;
			token = father->next;
		}
	}
	return (token);
}

t_token *find_left_token(t_token *token, t_token *first)////////yggdrasil
{
	t_token *eof;
	int i;

	i = 0;
	if (token == first || !token || !first)
		return(NULL);
	eof = token;
	token = get_prev_node(token, first);
	while (1)
	{
		if(eof->rank - i == token->rank)
			return (token);
		//?????????
		/*if(eof->rank - i < token->rank )
			return (NULL);*/
		//?????????
		if (eof->rank - i == RANK_F)
			return (NULL);
		token = get_prev_node(token, first);
		if(token == NULL)
		{
			i++;
			token = get_prev_node(eof, first);
		}
	}
	return (token);
}

t_node_tree *new_yggnode(t_token *token)////////yggdrasil
{
    t_node_tree *new_node;

    new_node = malloc(sizeof(t_node_tree));
    if (!new_node)
        return (NULL);
	if (!token)
        return (NULL);
	if (token->used == true)
		return (NULL);
	token->used = true;
	new_node->right = NULL;
	new_node->left = NULL;
	//if (!new_node->right || !new_node->left)
	//	return (NULL);
	new_node->content = token->value;
	new_node->type = (t_ast_type)token->type;
	new_node->rank = token->rank;
	new_node->args = token->args;
	new_node->file = token->file;
    return (new_node);
}

t_node_tree	*make_yggdrasil(t_token *t, t_token *f, t_token *e, t_node_tree *y)////////yggdrasil
{
	//validation_tree(t, f);
	//take_args();
	if(!y)
		y = new_yggnode(t);
	y->left = new_yggnode(find_left_token(t, f));
	y->right = new_yggnode(find_right_token(t, e));
	if (y->left)
		make_yggdrasil(find_left_token(t, f), f, t, y->left);
	if (y->right)
		make_yggdrasil(find_right_token(t, e), f, e, y->right);
	return (y);
}

bool token_err(t_token *token_lst)////////////////init_yggdrasil
{
	while (token_lst)
	{
		//printf("%d\n",token_lst->err);
		if(token_lst->err == 1)
			return (true);
		token_lst = token_lst->next;
	}
	return (false);
}

t_node_tree *init_yggdrasil(t_token *token_lst)////////////////init_yggdrasil
{
	t_token *root;
	t_token *first;

	root = token_lst;
	first = token_lst;
	if(token_err(token_lst))
		return(new_yggnode(new_eof(root)));
	while (token_lst)
	{
		if(root->rank <= token_lst->rank\
		 /*|| (token_lst->type == TOKEN_REDIR_OUT\
		  || token_lst->type == TOKEN_APPEND\
		   || token_lst->type == TOKEN_HEREDOC\
		    || token_lst->type == TOKEN_REDIR_IN)*/)
			root = token_lst;
		token_lst = token_lst->next;
	}
	//root->rank = RANK_S;
	return (make_yggdrasil(root, first, NULL, NULL));
}

void print_yggdrasil(t_node_tree *yggnode, int num_tabs, char *leg)///////print_yggdrasil
{
	char *arrow;
	char *tab = "         ";
	int i;

	i = 0;
	while(i++ < num_tabs)
		printf("%s", tab);
	num_tabs++;
	if((ft_strcmp("L:", leg) != 0 && ft_strcmp("R:",leg)) != 0)
		arrow = "---->";
	else
		arrow = "╰─-->";
	if(!yggnode)
		return ;
	printf("%s %s{ %s ", arrow, leg, yggnode->content);

	if(yggnode->args && !yggnode->file)
		printf("(args):[%s", yggnode->args[0]);
	else if (!yggnode->file)
		printf("}\n");
	if(yggnode->args)
		for (size_t i = 1; yggnode->args[i]; i++)
			printf(", %s", yggnode->args[i]);
	if(yggnode->file && !yggnode->args)
		printf("(file):[%s", yggnode->file);
	if(yggnode->args || yggnode->file)
		printf("] }\n");

	if(yggnode->right)
	{
		leg = "R:";
		print_yggdrasil(yggnode->right, num_tabs, leg);
	}
	if(yggnode->left)
	{
		leg = "L:";
		print_yggdrasil(yggnode->left, num_tabs, leg);	
	}
}

	

