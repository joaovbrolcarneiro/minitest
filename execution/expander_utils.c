/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* expander_utils.c                                   :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/* mplíčdeps   +#+           */
/* Created: 2025/04/15 00:00:00 by hde-barr          #+#    #+#             */
/* Updated: 2025/04/15 13:15:02 by hde-barr         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

/* Helper: Appends null-terminated string content to v->result */
static int	append_str_to_exp_vars(t_exp_vars *v, char *str)
{
	size_t	k;

	k = 0;
	if (!str)
		return (0);
	while (str[k])
	{
		if (!append_char(&v->result, &v->res_len, &v->res_cap, str[k]))
			return (1);
		k++;
	}
	return (0);
}

/* Appends expanded exit status ($?) to result */
/* Assumes append_str_to_exp_vars handles errors */
void	handle_exit_status(t_exp_vars *v)
{
	char	*exit_str;

	exit_str = ft_itoa(g_exit_code);
	if (!exit_str)
	{
		perror("minishell: ft_itoa");
		return ;
	}
	append_str_to_exp_vars(v, exit_str);
	free(exit_str);
	v->i++;
}

/* Helper: Extracts variable name length */
static size_t	get_var_name_len(const char *input, size_t start_index)
{
	size_t	current_pos;
	int		char_pos;

	current_pos = start_index;
	char_pos = 0;
	while (input[current_pos] && \
		is_valid_var_char(input[current_pos], char_pos))
	{
		current_pos++;
		char_pos++;
	}
	return (current_pos - start_index);
}

/* Appends expanded variable ($VAR) to result */
/* Assumes append_str_to_exp_vars handles errors */
void	handle_variable(t_exp_vars *v)
{
	char	*var_name;
	size_t	var_len;

	v->var_start = v->i;
	var_len = get_var_name_len(v->input, v->var_start);
	if (var_len == 0)
		return ;
	var_name = ft_substr(v->input, v->var_start, var_len);
	v->i += var_len;
	if (!var_name)
	{
		perror("minishell: ft_substr");
		return ;
	}
	v->var_value = get_env_value_exp(var_name, v->env);
	free(var_name);
	append_str_to_exp_vars(v, v->var_value);
}

/* Handles '$' expansion, returning 0 on success, 1 on failure */
int	handle_dollar_expansion(t_exp_vars *v)
{
	v->i++;
	if (v->input[v->i] == '?')
	{
		handle_exit_status(v);
	}
	else if (is_valid_var_char(v->input[v->i], 0))
	{
		handle_variable(v);
	}
	else
	{
		if (!append_char(&v->result, &v->res_len, &v->res_cap, '$'))
			return (1);
	}
	return (0);
}
