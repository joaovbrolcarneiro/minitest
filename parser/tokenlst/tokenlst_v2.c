/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenlst_v2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrol-ca <jbrol-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 21:05:42 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/16 22:17:25 by jbrol-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"
#include "minishell_part2.h"

int	ischarset(const char *set, char c);

size_t ft_strsetlen(char *s, const char *set);
 
size_t ft_strnlen(char *s, char n);


bool proximity_exception(char *input, int i)/////////split_input talvez mexi - joao
{
    if((!ischarset("|<>", input[i]) && input[i] != ' ') && input[i] && !ischarset("|<>", *input ))
        return (true);
    return (false);
}

t_token *new_pipe(t_token *token)////////////init_node1
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
	return (token);
}

t_token *new_redir(t_token *token)///////////////init_node1
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
	return (token);
}

t_token *new_cmd(t_token *token)///////////////////init_node1
{
	token->used = false;
	token->rank = RANK_B;
	token->id = get_token_id();
	token->type = TOKEN_CMD;
	token->value = token->value;
	token->args = NULL;
	token->file = NULL;
	token->coretype = TOKEN_CMD;
	return (token);
}

t_token *new_word(t_token *token)////////////////init_node1
{
	token->used = false;
	token->rank = RANK_C;
	token->id = get_token_id();
	token->type = TOKEN_WORD;
	token->value = token->value;
	token->args = NULL;
	token->file = NULL;
	token->coretype = TOKEN_WORD;
	return (token);
}

t_token *new_eof(t_token *token)//////////////////init_node1
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

t_token *new_token_super(t_token *token)////////////////init_node0
{
	return (token);
}

////////////////////////////////////////

bool is_redir_super(char *input)//////////who_is_node
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

bool is_word_super(char *input)//////////who_is_node (talvez mexi - joao)
{
    // Mark 'input' parameter as unused to avoid warning/error
    (void)input;
    // Any non-operator/non-command token is considered a word by default
    return (true);
}


bool is_pipe_super(char *input)//////////who_is_node
{
	if(ft_strlen(input) != 1)
		return(false);
	if(*input == '|')
		return(true);
	return(false);
}

bool is_cmd_super(char *input, char **env)
{
    if (is_builtin(input))
    {
        return (true);
    }
    return (search_list(input, env));
}

bool is_eof(char *input)//////////who_is_node
{
	if (!input)
		return (true);
	return (false);
}

/////////////////////////////////////////////// MEXI NESSA FUNCAO

t_token *typealize(t_token *token, char **env)///////
{
	if (token->type == TOKEN_ASSIGNMENT) // coloquei isso de resto mudei nada
		return (token); 

	if(is_pipe_super(token->value))
		return(new_token_super(new_pipe(token)));
	if(is_redir_super(token->value))
		return(new_token_super(new_redir(token)));
	if(is_cmd_super(token->value, env))
		return(new_token_super(new_cmd(token)));
	if(is_word_super(token->value))
		return(new_token_super(new_word(token)));
	if(is_eof(token->value))
		return (new_token_super(new_eof(token)));
	return (token);
}

/* A FUNCAO ABAIXO AGORA ESTA EM TOKENLST_SPLIT_INPUT
t_token	*split_input(char *input, int i) /////////split_input ( mexi-joao)
{
	t_token	*lst;
	t_token	*first;
	char	start_char;

	lst = malloc(sizeof(t_token));
	if (!lst)
		return (perror("malloc split_input"), NULL);
	first = lst;
	while (*input)
	{
		i = 1;
		while (*input && *input == ' ')
			input++;
		if (!*input)
		{
			lst->next = NULL;
			lst->value = NULL;
			break ;
		}
		start_char = *input;
		if (ischarset("\"'", start_char))
		{
			i = 1;
			while (input[i] && input[i] != start_char)
				i++;
			if (input[i] == start_char)
				i++;
			else
				i = 1;
		}
		else
		{
			i = ft_strsetlen(input, "\"' |<>");
			if (i == 0 && ischarset("|<>", start_char))
			{
				i = 1;
				if (ischarset("<>", start_char) && input[1] == start_char)
					i = 2;
			}
			else if (i == 0 && *input != '\0')
				i = 1;
			else if (i == 0 && *input == '\0')
				break ;
		}
		if (i == 0)
			i = 1;
		lst->merge_next = proximity_exception(input, i);
		lst->value = ft_substr(input, 0, i);
		if (!lst->value)
		{
			perror("ft_substr split_input");
			break ;
		}
		input += i;
		if (*input)
		{
			lst->next = malloc(sizeof(t_token));
			if (!lst->next)
			{
				perror("malloc next split_input");
				free(lst->value);
				lst->value = NULL;
				break ;
			}
			lst = lst->next;
		}
		else
		{
			lst->next = NULL;
			break ;
		}
	}
	if (lst && !*input)
		lst->next = NULL;
	if (!first || !first->value) 
	{
		if (first)
			free(first);
		return (NULL);
	}
	return (first);
}*/