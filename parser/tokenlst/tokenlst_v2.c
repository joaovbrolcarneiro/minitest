/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenlst_v2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrol-ca <jbrol-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 21:05:42 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/14 18:59:53 by jbrol-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"
#include "minishell_part2.h"

int	ischarset(const char *set, char c);

size_t ft_strsetlen(char *s, const char *set);
 
size_t ft_strnlen(char *s, char n);


bool proximity_exception(char *input, int i)/////////split_input
{
    // (Your existing logic)
    if((!ischarset("|<>", input[i]) && input[i] != ' ') && input[i] && !ischarset("|<>", *input ))
        return (true);
    return (false);
}

t_token *split_input(char *input, int i)/////////split_input (MODIFIED)
{
    t_token *lst;
    // t_token *next; // <<< REMOVED UNUSED VARIABLE
    t_token *first;

    lst = malloc(sizeof(t_token));
    if (!lst) // Add check for malloc failure
        return (perror("malloc split_input"), NULL);
    first = lst;
    while(*input)
    {
        i = 1;
        while (*input && *input == ' ')
            input++;
        if(!*input) { // Reached end after spaces
            // Need to handle the last allocated node if it wasn't used
            // Maybe adjust logic slightly to malloc inside loop?
            // For now, let's assume current logic intends to return 'first'
            // but ensure the last node is properly terminated/handled
            // If the loop breaks here, 'lst' might point to uninitialized memory
            // Let's refine: Only return 'first' if at least one token was made.
            // If input is just spaces, handle cleanly.
            // A simpler approach might be needed here, but sticking to current structure:
             lst->next = NULL; // Ensure last node points to NULL
             lst->value = NULL; // Ensure value is NULL if loop exits early
             // Need to free 'lst' if it was the first node and no token was processed?
             // Let's assume caller handles potential NULL return if input is just spaces.
             // Current return below assumes at least one token is processed.
             break; // Break loop instead of returning immediately?
        }

        // Determine token length 'i' based on quotes or delimiters
        char start_char = *input; // Keep track of starting char
        if(ischarset("\"'", start_char))
        {
            i = 1;
            while(input[i] && input[i] != start_char) i++;
            if (input[i] == start_char) i++;
            else { /* Handle unclosed quote error if needed */ i = 1; /* Default? */ }
        }
        else
        {
            i = ft_strsetlen(input, "\"' |<>");
            if (i == 0 && ischarset("|<>", start_char)) { // Handle operators
                i = 1; // Operator length is 1
                if (ischarset("<>", start_char) && input[1] == start_char)
                    i = 2; // Double operator length is 2
            } else if (i==0 && *input != '\0') { // Catch other single non-delimiter chars?
                 i = 1; // Should not happen if ft_strsetlen is correct?
            } else if (i == 0 && *input == '\0') { // End of string case
                break; // Nothing left to process
            }
        }
         if (i == 0) i = 1; // Ensure progress if logic fails

        lst->merge_next = proximity_exception(input, i);
        lst->value = ft_substr(input, 0, i);
        if (!lst->value) { /* Malloc failed in ft_substr */ perror("ft_substr split_input"); break; }

        // Prepare for next iteration or end
        input += i;
        if(*input) { // If more input remains
            lst->next = malloc(sizeof(t_token));
            if (!lst->next) { /* Malloc failed */ perror("malloc next split_input"); free(lst->value); break; }
            lst = lst->next;
        } else { // End of input
            lst->next = NULL;
            break;
        }
    }
    // If loop broke early due to error, ensure last node is NULL terminated
    if (lst) lst->next = NULL; // Ensure final node is terminated

    // Check if any tokens were actually processed
    if (!first || !first->value) {
        // Handle empty input or only spaces - free initial malloc if nothing assigned
        if (first) free(first);
        return NULL;
    }

    return (first);
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
	//token->next = NULL;
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
	//token->next = NULL;
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
	//token->next = NULL;
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
	//token->next = NULL;
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

bool is_word_super(char *input)//////////who_is_node (MODIFIED)
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
    // 1. Check builtins FIRST
    if (is_builtin(input))
    {
        return (true);
    }
    // 2. Check PATH only if not a builtin
    // Assuming search_list checks the PATH and executable status correctly
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
