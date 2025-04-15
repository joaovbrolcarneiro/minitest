/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* expander_utils2.c                                  :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/*<y_bin_423>+#+#+#+#+#+   +#+           */
/* Created: 2025/04/15 13:15:00 by hde-barr          #+#    #+#             */
/* Updated: 2025/04/15 13:15:01 by hde-barr         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

/* Helper for append_char: Reallocates buffer if needed */
static int	realloc_exp_buffer(char **buf, size_t len, size_t *cap)
{
	size_t	i;
	char	*new_buf;
	size_t	new_cap;

	new_cap = (*cap) * 2;
	if (new_cap == 0)
		new_cap = 128;
	new_buf = malloc(new_cap);
	if (!new_buf)
		return (perror("minishell: expander realloc"), 0);
	i = 0;
	if (*buf)
	{
		while (i < len)
		{
			new_buf[i] = (*buf)[i];
			i++;
		}
		free(*buf);
	}
	*buf = new_buf;
	*cap = new_cap;
	return (1);
}

/* Appends char to dynamically sized buffer used by expander */
/* Returns 1 on success, 0 on failure (malloc) */
int	append_char(char **buf, size_t *len, size_t *cap, char c)
{
	if (!buf || !len || !cap)
		return (0);
	if (!(*buf) || *len + 1 >= *cap)
	{
		if (!realloc_exp_buffer(buf, *len, cap))
			return (0);
	}
	(*buf)[(*len)] = c;
	(*len)++;
	(*buf)[(*len)] = '\0';
	return (1);
}

int	append_normal_char(t_exp_vars *v)
{
	if (!append_char(&v->result, &v->res_len, &v->res_cap, v->input[v->i]))
		return (1);
	v->i++;
	return (0);
}
