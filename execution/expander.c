/* ************************************************************************** */
/* expander.c                                                                 */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value_exp(const char *name, char **env)
{
	char	*val;

	val = get_env_value(env, name);
	if (!val)
		return ("");
	return (val);
}

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

char    *expand_variables(const char *input, char **env /* REMOVED , int last_exit_status */)
{
    t_exp_vars  v;
    char        *original_input_ptr = (char *)input;

    if (!input) return (NULL);

    // Call init without passing status
    exp_var_init(&v, input, env /* REMOVED , last_exit_status */);

    v.result = malloc(v.res_cap);
    if (!v.result) return (NULL);
    v.result[0] = '\0';

    bool expansion_occurred = false;
    while (v.input[v.i])
    {
        if (v.input[v.i] == '$') {
            expansion_occurred = true;
            handle_dollar_expansion(&v); // handle_exit_status inside reads global
        } else {
            if (!append_char(&v.result, &v.res_len, &v.res_cap, v.input[v.i])) {
                free(v.result); return (NULL);
            }
            v.i++;
        }
    }
    if (!append_char(&v.result, &v.res_len, &v.res_cap, '\0')) {
        free(v.result); return (NULL);
    }
    return (v.result);
}

void	expand_token_list(t_token *token_list, char **env)
{
	t_token	*cur;
	char	*expanded;

	cur = token_list;
	while (cur)
	{
		if (cur->value && !cur->literal)
		{
			expanded = expand_variables(cur->value, env);
			free(cur->value);
			cur->value = expanded;
		}
		cur = cur->next;
	}
}

void    exp_var_init(t_exp_vars *v, const char *input, char **env /* REMOVED , int last_exit_status */)
{
    v->input = input;
    v->env = env;
    // v->last_exit_status = last_exit_status; // REMOVED
    v->i = 0;
    v->res_len = 0;
    v->res_cap = 128;
    v->result = NULL;
    v->var_start = 0;
    v->pos = 0;
    v->var_value = NULL;
}