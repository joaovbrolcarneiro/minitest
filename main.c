/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* main.c                                             :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/04/15 00:00:00 by hde-barr          #+#    #+#             */
/* Updated: 2025/04/15 00:00:01 by hde-barr         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_part2.h"

int	g_exit_code = 0;

/*
** konopwd:
** - If the trimmed input equals "pwd", prints the current directory.
*/
bool	konopwd(bool cmd_exist, const char *input)
{
	char	*buf;
	size_t	size;
	char	*trimmed;

	size = 4096;
	buf = malloc(size);
	if (!buf)
		return (cmd_exist);
	trimmed = ft_strtrim(input, " ");
	if (!trimmed)
	{
		free(buf);
		return (cmd_exist);
	}
	if (ft_strcmp(trimmed, "pwd") == 0)
	{
		if (getcwd(buf, size) != NULL)
		{
			ft_putstr_fd(RDK, 1);
			ft_putstr_fd(buf, 1);
			ft_putstr_fd("\n" RST, 1);
		}
		else
			perror("minishell: konopwd: getcwd");
		free(buf);
		free(trimmed);
		return (true);
	}
	free(trimmed);
	free(buf);
	return (cmd_exist);
}

void	parser_cmd_no_found(t_token *token, char **env)
{
	t_token_type	type;

	while (token)
	{
		type = token->type;
		if (type != TOKEN_PIPE && type != TOKEN_ASSIGNMENT && \
			type != TOKEN_REDIR_IN && type != TOKEN_REDIR_OUT && \
			type != TOKEN_APPEND && type != TOKEN_HEREDOC)
		{
			if (token->rank <= RANK_B && \
				!search_list(token->value, env) && \
				!is_builtin(token->value))
			{
				token->type = TOKEN_WORD;
			}
			else if (token->type == TOKEN_WORD && \
					(is_builtin(token->value) || search_list(token->value, env)))
			{
				token->type = TOKEN_CMD;
				token->rank = RANK_B;
				token->coretype = TOKEN_CMD;
			}
		}
		token = token->next;
	}
}

t_token	*rm_node_lst(t_token *token, t_token *first)
{
	t_token	*prev;

	if (!token || !first)
		return (NULL);
	prev = get_prev_node(token, first);
	if (!prev)
		return (NULL);
	prev->next = token->next;
	token->next = NULL;
	return (token);
}

void	add_node_lst(t_token *dst_lst, t_token *token)
{
	t_token	*tmp;

	if (!dst_lst || !token)
		return ;
	tmp = dst_lst->next;
	dst_lst->next = token;
	token->next = tmp;
}

void	join_and_split(t_token *priv, t_token *token) // ALTEREI - JOAO
{
	char	*temp_join1;
	char	*temp_join2;
	char	**args_to_free;

	if (!priv || !token || !token->value)
		return ;
	args_to_free = priv->args;
	if (!priv->value)
		priv->value = ft_strdup("");
	if (!priv->value)
		return ;
	temp_join1 = ft_strjoin(priv->value, "\x1F");
	if (!temp_join1)
		return (perror("minishell: join_and_split: strjoin1"));
	temp_join2 = ft_strjoin(temp_join1, token->value);
	free(temp_join1);
	if (!temp_join2)
		return (perror("minishell: join_and_split: strjoin2"));
	priv->args = ft_split(temp_join2, '\x1F');
	free(temp_join2);
	if (args_to_free)
		ft_free_strarray(args_to_free);
	if (!priv->args)
		perror("minishell: join_and_split: ft_split");
}

t_token	*redir_handler_file(t_token *token, t_token *first)
{
	t_token	*file_node;

	if (!token || token->coretype != REDIR || !token->next || \
		token->next->rank == RANK_S)
		return (token);
	file_node = rm_node_lst(token->next, first);
	if (file_node)
	{
		token->file = file_node->value;
	}
	else
	{
		token->file = NULL;
	}
	return (token);
}

t_token	*cmd_handler_args(t_token *token, t_token *first)
{
	t_token	*redir;
	t_token	*arg_node;

	redir = NULL;
	while (token && token->rank != RANK_S)
	{
		while (token->next && token->next->rank != RANK_S)
		{
			if (token->next->coretype == REDIR)
				break ;
			else
			{
				arg_node = rm_node_lst(token->next, first);
				if (!arg_node)
					break ;
				join_and_split(token, arg_node);
			}
		}
		if (redir)
			add_node_lst(token, redir);
		if (redir)
			redir = NULL;
		token = token->next;
	}
	return (first);
}

t_token	*handler_args_file(t_token *token, t_token *first)
{
	t_token	*current;

	current = token;
	while (current)
	{
		redir_handler_file(current, first);
		cmd_handler_args(current, first);
		current = current->next;
	}
	return (first);
}

t_token	*last_token(t_token *token)
{
	if (!token)
		return (NULL);
	while (token->next)
		token = token->next;
	return (token);
}

void	handler_pipes(t_token_vec *token_vec)
{
	if (token_vec)
	{
		if (token_vec->so_far)
		{
			if (token_vec->so_far->type == TOKEN_PIPE)
			{
				if (token_vec->first)
					last_token(token_vec->first)->next = token_vec->so_far;
			}
		}
	}
}

t_token	*remap_lst(t_token *token) // mexi - joao
{
	t_token		*first;
	t_token_vec	*token_vec;

	first = NULL;
	token_vec = NULL;
	while (token)
	{
		handler_pipes(token_vec);
		if (first == NULL)
		{
			if (token_vec)
				first = token_vec->first;
		}
		if (token_vec)
			token = token_vec->so_far;
		else
			token = NULL;
		if (token)
		{
			if (token->type == TOKEN_PIPE)
				token = token->next;
			if (token)
				token = token->next;
		}
	}
	return (first);
}

void	handle_ctrl_c(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	is_minishell_exit(char *input)
{
	char	*res;

	res = ft_strtrim(input, " ");
	if (!res)
		return ;
	if (ft_strcmp("exit", res) == 0)
	{
		free(res);
		exit(0);
	}
	free(res);
}

bool	merge_to_token_exception(t_token *token)
{
	t_token	*first;
	t_token	*rm_node;
	char	*temp_val;

	first = token;
	if (!token || !token->next || token->merge_next == false)
		return (false);
	rm_node = rm_node_lst(token->next, first);
	if (!rm_node)
		return (false);
	temp_val = token->value;
	token->value = ft_strjoin(temp_val, rm_node->value);
	if (!token->value)
	{
		token->value = temp_val;
		return (false);
	}
	return (true);
}

/* Helper function to check for parser errors in token list */
bool	has_parser_error(t_token *token)
{
	while (token)
	{
		if (token->err)
			return (true);
		token = token->next;
	}
	return (false);
}

/*
** input_handler:
ALTEREI UM POUCO - joao
*/
void	input_handler(t_shell *shell, char *input)
{
	t_token		*token;
	t_node_tree	*tree;
	bool		parse_error;

	tree = NULL;
	token = delegated_by_input_handler(input, shell->env);
	parse_error = has_parser_error(token);
	if (!parse_error)
	{
		process_variable_assignments(shell, token);
		expand_token_list_no_assignments(token, shell->env);
		token = handler_args_file(token, token);
		parser_cmd_no_found(token, shell->env);
		tree = init_yggdrasil(token);
		if (tree)
			execute_ast(shell, tree);
		else if (!has_parser_error(token))
			g_exit_code = 0;
	}
	else
	{
		g_exit_code = 2;
	}
	// free_tokens(token); free?
	// free_ast(tree); free?
}

t_token	*delegated_by_input_handler(char *input, char **env)
{
	t_token	*first;
	t_token	*token;

	token = split_input(input, 0);
	first = token;
	quote_handler_call_loop(token, env);
	typealize_call_loop(token, env);
	merg_tok_excep_cll_revloop(token);
	return (first);
}

void	typealize_call_loop(t_token *token, char **env)
{
	t_token	*current_token;

	current_token = token;
	while (current_token)
	{
		typealize(current_token, env);
		current_token = current_token->next;
	}
}

void	merg_tok_excep_cll_revloop(t_token *token)
{
	t_token	*first;
	t_token	*current;

	first = token;
	current = last_token(first);
	while (current && current != first)
	{
		token = get_prev_node(current, first);
		if (token)
			merge_to_token_exception(token);
		current = token;
	}
	if (first)
		merge_to_token_exception(first);
}

void	quote_handler_call_loop(t_token *token, char **env)
{
	int	is_unclosed;

	while (token)
	{
		is_unclosed = 0;
		token->value = quote_handler(token, env, &is_unclosed);
		if (is_unclosed == 1)
			token->err = 1;
		else
			token->err = 0;
		token = token->next;
	}
}

/*
** readline_loop:
** The main loop: prints the prompt, reads input, and processes it.
*/
void	readline_loop(t_shell *shell)
{
	char	*input;

	while (1)
	{
		// Changed ft_printf to putstr for norm
		ft_putstr_fd("💥", 1);
		konopwd(true, "pwd"); // Prints path and newline
		input = readline(TITLE); // Prints prompt
		if (!input)
		{
			ft_putstr_fd("exit\n", STDOUT_FILENO);
			cleanup_shell(shell);
			exit(g_exit_code);
		}
		if (*input)
			add_history(input);
		if (!ft_strlen(input))
		{
			free(input);
			continue ;
		}
		input_handler(shell, input);
		free(input);
	}
}

/*
** main:
*/
int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	g_exit_code = 0;
	init_shell(&shell, envp);
	signal(SIGINT, handle_ctrl_c);
	signal(SIGQUIT, SIG_IGN);
	readline_loop(&shell);
	cleanup_shell(&shell);
	return (g_exit_code);
}

bool	is_executable(const char *path)
{
	if (access(path, F_OK | X_OK) == 0)
		return (true);
	return (false);
}

bool	is_regular_file(const char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) != 0)
	{
		return (false);
	}
	return (S_ISREG(path_stat.st_mode));
}

bool	is_valid_exc(const char *path)
{
	return (is_regular_file(path) && is_executable(path));
}

bool	is_builtin(const char *cmd)
{
	if (!cmd)
		return (false);
	if (ft_strcmp(cmd, "echo") == 0 || \
		ft_strcmp(cmd, "cd") == 0 || \
		ft_strcmp(cmd, "pwd") == 0 || \
		ft_strcmp(cmd, "export") == 0 || \
		ft_strcmp(cmd, "unset") == 0 || \
		ft_strcmp(cmd, "exit") == 0)
	{
		return (true);
	}
	return (false);
}
