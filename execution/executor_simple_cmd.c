/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* executor_simple_cmd.c                              :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/04/14 23:15:00 by hde-barr          #+#    #+#             */
/* Updated: 2025/04/15 10:10:00 by hde-barr         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

/* Allocates cmd_args array if node->args is NULL */
static char	**prepare_cmd_args(t_node_tree *node)
{
	char	**cmd_args;

	cmd_args = malloc(sizeof(char *) * 2);
	if (!cmd_args)
	{
		perror("minishell: malloc");
		return (NULL);
	}
	cmd_args[0] = node->content;
	cmd_args[1] = NULL;
	return (cmd_args);
}

/* Performs initial checks on the node for execute_simple_command */
/* Returns 0 if OK, 1 on error */
static int	check_simple_cmd_node(t_node_tree *node)
{
	if (!node)
	{
		ft_putstr_fd("minishell: Error: execute_simple_command: NULL\n", 2);
		return (1);
	}
	if (node->type != AST_COMMAND)
	{
		ft_putstr_fd("minishell: Error: execute_simple_command node type ", 2);
		ft_putnbr_fd(node->type, 2);
		ft_putstr_fd("\n", 2);
		return (1);
	}
	return (0);
}

/* Executes simple command node (must be AST_COMMAND) */
/* Does not handle FD saving/restoring */
int	execute_simple_command(t_shell *shell, t_node_tree *node)
{
	int		status;
	char	**cmd_args;
	bool	args_malloced;

	if (check_simple_cmd_node(node) != 0)
		return (1);
	cmd_args = NULL;
	args_malloced = false;
	if (node->args && node->args[0])
		cmd_args = node->args;
	else if (node->content)
	{
		cmd_args = prepare_cmd_args(node);
		if (!cmd_args)
			return (1);
		args_malloced = true;
	}
	else
		return (0);
	status = execute_command(shell, cmd_args);
	if (args_malloced)
		free(cmd_args);
	return (status);
}
