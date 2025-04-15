/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* expander.c                                         :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/04/15 00:00:00 by hde-barr          #+#    #+#             */
/* Updated: 2025/04/15 18:35:00 by hde-barr         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

/* Gets value from environment, returns "" if not found */
char	*get_env_value_exp(const char *name, char **env)
{
	char	*val;

	val = get_env_value(env, name);
	if (!val)
		return ("");
	return (val);
}

/* Checks if char c is valid at position pos in a variable name */
int	is_valid_var_char(char c, int pos)
{
	if (pos == 0)
	{
		return ((c >= 'A' && c <= 'Z')
			|| (c >= 'a' && c <= 'z')
			|| c == '_');
	}
	return ((c >= 'A' && c <= 'Z')
		|| (c >= 'a' && c <= 'z')
		|| (c >= '0' && c <= '9')
		|| c == '_');
}

/* Helper loop for expand_variables */
/* Returns 0 on success, 1 on failure (e.g. append_char) */
static int	expansion_loop(t_exp_vars *v)
{
	while (v->input[v->i])
	{
		if (v->input[v->i] == '$')
		{
			if (handle_dollar_expansion(v) != 0)
				return (1);
		}
		else
		{
			if (append_normal_char(v) != 0)
				return (1);
		}
	}
	return (0);
}

/* Expands variables ($VAR, $?) in a single string */
/* Returns newly allocated string or NULL on error */
char	*expand_variables(const char *input, char **env)
{
	t_exp_vars	v;
	int			loop_status;

	if (!input)
		return (NULL);
	if (exp_var_init(&v, input, env) != 0)
		return (NULL);
	loop_status = expansion_loop(&v);
	if (loop_status != 0)
		return (free(v.result), NULL);
	if (!append_char(&v.result, &v.res_len, &v.res_cap, '\0'))
		return (free(v.result), perror("minishell: expand null term"), NULL);
	return (v.result);
}

/* Applies expand_variables to relevant tokens in a list */
/* Uses safer free/assign pattern */
void	expand_token_list(t_token *token_list, char **env)
{
	t_token	*cur;
	char	*expanded;
	char	*original_value;

	cur = token_list;
	while (cur)
	{
		if (cur->value && !cur->literal)
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
