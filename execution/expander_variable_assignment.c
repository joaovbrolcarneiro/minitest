/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* expander_variable_assignment.c                     :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/* mplíčdeps   +#+           */
/* Created: 2025/04/15 00:00:00 by hde-barr          #+#    #+#             */
/* Updated: 2025/04/15 18:45:00 by hde-barr         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

/* Checks if a token represents a valid potential assignment "VAR=..." */
int	is_valid_assignment(const char *token)
{
	int	i;

	i = 0;
	if (!token || (!ft_isalpha(token[i]) && token[i] != '_'))
		return (0);
	i++;
	while (token[i] && token[i] != '=')
	{
		if (!(ft_isalnum(token[i]) || token[i] == '_'))
			return (0);
		i++;
	}
	if (token[i] == '=')
		return (1);
	return (0);
}

/* Applies valid assignments at list start to env and marks tokens */
void	apply_variable_assignment(t_shell *shell, t_token *token_list)
{
	t_token	*current;
	char	*equal_sign;

	current = token_list;
	while (current && is_valid_assignment(current->value))
	{
		equal_sign = ft_strchr(current->value, '=');
		if (equal_sign)
		{
			*equal_sign = '\0';
			if (update_env(&shell->env, current->value, equal_sign + 1) != 0)
			{
				perror("minishell: update_env");
			}
			*equal_sign = '=';
		}
		current->type = TOKEN_ASSIGNMENT;
		current = current->next;
	}
}

/* Expands variables in tokens, skipping literals and assignments */
void	expand_token_list_no_assignments(t_token *token_list, char **env)
{
	t_token	*cur;
	char	*expanded;
	char	*original_value;

	cur = token_list;
	while (cur)
	{
		if (cur->value && !cur->literal && cur->type != TOKEN_ASSIGNMENT)
		{
			original_value = cur->value;
			expanded = expand_variables(original_value, env);
			if (expanded == NULL && original_value != NULL)
				ft_putstr_fd("minishell: warning: expansion failed\n", 2);
			else if (expanded != original_value)
			{
				free(original_value);
				cur->value = expanded;
			}
		}
		cur = cur->next;
	}
}

/* Main function to process assignments before other parsing/expansion */
void	process_variable_assignments(t_shell *shell, t_token *token_list)
{
	apply_variable_assignment(shell, token_list);
}

/* NOTE: This function logically belongs in expander.c */
/* Initializes the expander variables struct, returns 0 on success, 1 on fail */
int	exp_var_init(t_exp_vars *v, const char *input, char **env)
{
	v->input = input;
	v->env = env;
	v->i = 0;
	v->res_len = 0;
	v->res_cap = 128;
	v->result = NULL;
	v->var_start = 0;
	v->pos = 0;
	v->var_value = NULL;
	v->result = malloc(v->res_cap);
	if (!v->result)
		return (perror("minishell: exp_var_init malloc"), 1);
	v->result[0] = '\0';
	return (0);
}
