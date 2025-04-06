/* ************************************************************************** */
/* expander_utils.c                                                           */
/* ************************************************************************** */

#include "minishell.h"


int	append_char(char **buf, size_t *len, size_t *cap, char c)
{
	size_t	i;
	char	*new_buf;

	if (*len + 1 >= *cap)
	{
		new_buf = malloc((*cap) * 2);
		if (!new_buf)
			return (0);
		i = 0;
		while (i < *len)
		{
			new_buf[i] = (*buf)[i];
			i++;
		}
		free(*buf);
		*buf = new_buf;
		*cap *= 2;
	}
	(*buf)[(*len)++] = c;
	return (1);
}

void	handle_exit_status(t_exp_vars *v)
{
	char	*exit_str;
	size_t	k;

	exit_str = ft_itoa(v->last_exit_status);
	k = 0;
	while (exit_str[k])
	{
		if (!append_char(&v->result, &v->res_len, &v->res_cap, exit_str[k]))
		{
			free(exit_str);
			return ;
		}
		k++;
	}
	free(exit_str);
	v->i++;
}

void	handle_variable(t_exp_vars *v)
{
	char	*tmp;
	size_t	j;

	v->var_start = v->i;
	v->pos = 0;
	while (v->input[v->i] && is_valid_var_char(v->input[v->i], v->pos++))
		v->i++;
	tmp = ft_substr(v->input, v->var_start, v->i - v->var_start);
	v->var_value = get_env_value_exp(tmp, v->env);
	free(tmp);
	j = 0;
	while (v->var_value[j])
	{
		if (!append_char(&v->result, &v->res_len, &v->res_cap, v->var_value[j]))
			return ;
		j++;
	}
}

void	handle_dollar_expansion(t_exp_vars *v)
{
	v->i++;
	if (v->input[v->i] == '?')
		handle_exit_status(v);
	else if (is_valid_var_char(v->input[v->i], 0))
		handle_variable(v);
	else
	{
		if (!append_char(&v->result, &v->res_len, &v->res_cap, '$'))
			return ;
	}
}

void	append_normal_char(t_exp_vars *v)
{
	if (!append_char(&v->result, &v->res_len, &v->res_cap, v->input[v->i]))
		return ;
	v->i++;
}