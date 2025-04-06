/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 17:59:46 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/02 19:45:42 by hde-barr         ###   ########.fr       */
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

char *is_quote_opened(char *input, int *is_unclosed)///////it is here
{
	//char *buf;
	//char *tmp;
	//int i;
	//char c;

	//i = -1;
	// = *input;
	//buf = ft_strjoin(input,"\n");
	if(ischarset("\"'", *input))
		if(*input != input[ft_strlen(input) - 1] || ft_strlen(input) == 1)
			return(printf(RED"parser error: unclosed quotes\n"RESET), \
			*is_unclosed = 1, "unclosed_error");
			//return ("'parser error: unclosed quotes'");///////!!!!!!!!!!!!! joao util heredoc!!!!!!!!!!!!!!!!!!
			/*while (1)
			{
				ft_printf(BLUE">"RESET);
					tmp = get_next_line(0);
					buf = ft_strjoin(buf,tmp);
					if(tmp[ft_strnlen(tmp, '\n') -1 ] == c)
					{						
						if(buf[ft_strlen(buf) -1] == '\n')
							buf[ft_strlen(buf) -1] = '\0';
						return (buf);
					}
			}*/
	return (input);
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

char  *quote_handler(t_token *token, char **env, int *is_unclosed)///////it is here
{
	int cmd_len;

	*is_unclosed = 0;
	token->value = is_quote_opened(token->value, is_unclosed);
	if(handler_quote_operator(token->value))
		token->literal = true;
	else
		token->literal = false;
	if(*token->value == '\'')
		return (ft_strtrim(token->value, "'"));
	token->value = ft_strtrim(token->value, "\"");
	cmd_len = quote_handler_counter(token->value, env);
	token->value = quote_handler_cpy( cmd_len, token->value, env);
	return (token->value);
}

