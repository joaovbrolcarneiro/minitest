/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* quote_handling.c                                   :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/*<y_bin_46>+#+#+#+#+#+   +#+           */
/* Created: 2025/02/27 17:59:46 by hde-barr          #+#    #+#             */
/* Updated: 2025/04/16 20:45:00 by hde-barr         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_part2.h"

/* Checks for unclosed quotes */
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
			ft_putstr_fd("'\n" RESET, 2);
			*is_unclosed = 1;
		}
	}
	return (input);
}

/* Checks if input exactly matches a quoted operator */
bool	handler_quote_operator(char *input)
{
	int			i;
	const char	*dictionary[] = {
		"\"<<\"", "\">>\"", "\"<\"", "\">\"",
		"'<<'", "'>>'", "'<'", "'>'",
		"\"|\"", "'|'", NULL};

	i = 0;
	if (!input)
		return (false);
	while (dictionary[i])
	{
		if (ft_strcmp(dictionary[i], input) == 0)
			return (true);
		i++;
	}
	return (false);
}

/* Helper for quote_handler: Handles single quotes */
static char	*handle_single_quotes(t_token *token, char *original_value)
{
	char	*trimmed_value;

	token->literal = true;
	trimmed_value = ft_strtrim(original_value, "'");
	if (!trimmed_value)
	{
		perror("ft_strtrim in quote_handler");
		return (original_value);
	}
	// free(original_value); // Freeing original value commented out
	token->value = trimmed_value;
	return (token->value);
}

/* Helper for quote_handler: Handles double quotes */
static char	*handle_double_quotes(t_token *token, char **env, \
									char *original_value, bool is_operator_literal)
{
	char	*trimmed_value;
	char	*expanded_value;
	int		cmd_len;

	token->literal = is_operator_literal;
	trimmed_value = ft_strtrim(original_value, "\"");
	// free(original_value); // Freeing original value commented out
	if (!trimmed_value)
	{
		perror("ft_strtrim in quote_handler");
		token->value = NULL;
		return (NULL);
	}
	token->value = trimmed_value;
	if (!token->literal)
	{
		cmd_len = quote_handler_counter(trimmed_value, env);
		expanded_value = quote_handler_cpy(cmd_len, trimmed_value, env);
		// free(trimmed_value); // Freeing intermediate result commented out
		if (!expanded_value)
		{
			perror("quote_handler_cpy failed");
			token->value = NULL;
			return (NULL);
		}
		token->value = expanded_value;
	}
	return (token->value);
}

/* Main quote handler */
char	*quote_handler(t_token *token, char **env, int *is_unclosed)
{
	char	quote_char;
	char	*original_value;
	bool	is_operator_literal;

	*is_unclosed = 0;
	if (!token || !token->value)
		return (NULL);
	original_value = token->value;
	quote_char = 0;
	if (original_value[0] == '\'' || original_value[0] == '"')
		quote_char = original_value[0];
	is_quote_opened(original_value, is_unclosed);
	if (*is_unclosed)
		return (original_value);
	is_operator_literal = handler_quote_operator(original_value);
	if (quote_char == '\'')
		return (handle_single_quotes(token, original_value));
	else if (quote_char == '"')
		return (handle_double_quotes(token, env, original_value, \
									is_operator_literal));
	else
	{
		token->literal = is_operator_literal;
		return (original_value);
	}
}

