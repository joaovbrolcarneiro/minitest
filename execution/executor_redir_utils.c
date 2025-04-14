/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* executor_redir_utils.c                             :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/04/14 22:00:00 by hde-barr          #+#    #+#             */
/* Updated: 2025/04/14 23:59:00 by hde-barr         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

/* Saves original STDIN/STDOUT and returns status (0=ok, 1=error) */
int	save_original_fds(int original_fds[2])
{
	original_fds[0] = dup(STDIN_FILENO);
	original_fds[1] = dup(STDOUT_FILENO);
	if (original_fds[0] == -1 || original_fds[1] == -1)
	{
		perror("minishell: dup original fds");
		if (original_fds[0] != -1)
			close(original_fds[0]);
		return (1);
	}
	return (0);
}

/* Finds and executes the command node associated with a redirection node */
int	execute_redir_cmd_node(t_shell *shell, t_node_tree *redir_node)
{
	t_node_tree	*cmd_node;
	int			status;

	status = 0;
	cmd_node = NULL;
	if (!redir_node)
		return (1);
	cmd_node = redir_node->left;
	if (!cmd_node || cmd_node->type != AST_COMMAND)
	{
		if (redir_node->right && redir_node->right->type == AST_COMMAND)
			cmd_node = redir_node->right;
		else
			cmd_node = NULL;
	}
	if (cmd_node)
		status = execute_ast(shell, cmd_node);
	else
	{
		ft_putstr_fd("minishell: syntax error near redirection\n", 2);
		status = 2;
	}
	return (status);
}

/* Static helper to dispatch execution based on node type */
/* Called only by execute_ast within this file */
static int	dispatch_ast_node(t_shell *shell, t_node_tree *node)
{
	int	status;

	status = 0;
	if (node->type == AST_PIPE)
		status = handle_pipe_execution(shell, node);
	else if (node->type >= AST_REDIR_IN && node->type <= AST_HEREDOC)
		status = handle_redir_execution(shell, node);
	else if (node->type == AST_COMMAND)
		status = handle_command_execution(shell, node);
	else if (node->type == (t_ast_type)TOKEN_ASSIGNMENT)
		status = handle_assignment_execution(node);
	else if (node->type == (t_ast_type)TOKEN_WORD)
		status = handle_word_token_execution(node);
	else
	{
		ft_putstr_fd("minishell: execute_ast: Unknown node type ", 2);
		ft_putnbr_fd(node->type, 2);
		ft_putstr_fd("\n", 2);
		status = 1;
	}
	return (status);
}

/* Main execution entry point (lives here as requested by user) */
int	execute_ast(t_shell *shell, t_node_tree *node)
{
	int	status;

	status = 0;
	if (!node)
	{
		g_exit_code = 0;
		return (0);
	}
	status = dispatch_ast_node(shell, node);
	g_exit_code = status;
	return (status);
}

/* Handles execution attempt for TOKEN_WORD nodes */
int	handle_word_token_execution(t_node_tree *node)
{
	ft_putstr_fd("minishell: command not found: ", 2);
	if (node && node->content)
		ft_putstr_fd(node->content, 2);
	else
		ft_putstr_fd("unknown", 2);
	ft_putstr_fd("\n", 2);
	return (127);
}
