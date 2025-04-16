/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrol-ca <jbrol-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 17:59:46 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/16 20:30:54 by jbrol-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_part2.h"

//int	g_exit_code;



//void print_env(char **env);

int quote_handler_counter(char *input, char **env)/////it is here
{
	int i;
	int counter;
	int start;
	
	i = 0;
	counter = 0;
	while(input[i])
		if(ischarset("$", input[i]))
		{
			start = ++i;
			while (!ischarset(QUOTE_SET, input[i]) &&  input[i])
				i++;
			counter += ft_strlen(\
			domane_expantion(env, ft_substr(input, start, i - start)));
		}
		else
		{
			i++;
			counter++;
		}	
	return (counter);
}

char *quote_handler_cpy(int count, char *input, char **env)///////it is here
{
	char *dst;
	int i;
	int start;
 
	i = 0;
	if(ft_strcmp(ft_strtrim(input, " "),"$") == 0)
		return ("$");
	dst = ft_calloc(sizeof(char), count + 1);
	count = 0;
	if(!dst)
		return("");
	while (input[i])
		if(ischarset("$", input[i]))
		{
			start = ++i;
			while (!ischarset(QUOTE_SET, input[i]) &&  input[i])
				i++;
			ft_strlcat(dst, domane_expantion(env, ft_substr(input, start, \
			i - start)), count + ft_strlen(domane_expantion(env, \
			ft_substr(input, start, i - start))) + 1);
			count = ft_strlen(dst);
		}
		else
			dst[count++] = input[i++];
	return (dst);
}

char	*is_quote_opened(char *input, int *is_unclosed)
{
	size_t	len;
	char	start_quote;

	*is_unclosed = 0;
	if (!input)
		return (NULL);
	len = ft_strlen(input);
	if (ischarset("\"'", input[0]))
	{
		start_quote = input[0];
		if (len == 1 || input[0] != input[len - 1])
		{
			ft_putstr_fd(RED "konosubash: parser error: unclosed quote `", 2);
			ft_putstr_fd(input, 2);
			ft_putstr_fd("'\n" RESET, 2); // Use colors here too if desired
			*is_unclosed = 1;
			return (input); // Return original pointer on error
		}
	}
	return (input); // Return original pointer if okay
}

bool handler_quote_operator(char *input)///////it is here
{
	int i;
	const char *dictionary[] = \
	{"\"<<\"", "\">>\"", "\"<\"", "\">\"", "'<<'", "'>>'", \
	"'<'", "'>'", "\"|\"", "'|'", NULL};

	i = 0;
	while (dictionary[i])
	{
		if (ft_strcmp(dictionary[i], input) == 0)
			return (true);
		i++;
	}
	return (false);
}

char  *quote_handler(t_token *token, char **env, int *is_unclosed)
{
    int cmd_len;
    char quote_char = 0;
    char *trimmed_value = NULL;
    bool is_operator_literal;

    *is_unclosed = 0;
    if (!token || !token->value)
        return (NULL);

    if (token->value[0] == '\'' || token->value[0] == '"')
        quote_char = token->value[0];

    token->value = is_quote_opened(token->value, is_unclosed);
    if (*is_unclosed)
        return (token->value);

    is_operator_literal = handler_quote_operator(token->value);

    if (quote_char == '\'')
    {
        token->literal = true; // Change was adding this line
        trimmed_value = ft_strtrim(token->value, "'");
         if (!trimmed_value) {
            perror("ft_strtrim in quote_handler");
            return (token->value);
        }
        // free(token->value); // Consider memory management if value was allocated
        token->value = trimmed_value;
        return (token->value);
    }

    token->literal = is_operator_literal;

    if (quote_char == '"')
    {
        trimmed_value = ft_strtrim(token->value, "\"");
         if (!trimmed_value) {
            perror("ft_strtrim in quote_handler");
            return (token->value);
        }
        // free(token->value); // fazer free?
        token->value = trimmed_value;
        if (!token->literal) {
             cmd_len = quote_handler_counter(token->value, env);
             // Potential free needed for token->value before reassigning?
             token->value = quote_handler_cpy(cmd_len, token->value, env);
        }
        return (token->value);
    }

    return (token->value);
}


