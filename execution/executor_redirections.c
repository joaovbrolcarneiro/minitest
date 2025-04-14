/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* executor_redirections.c                          :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/04/14 00:00:00 by hde-barr          #+#    #+#             */
/* Updated: 2025/04/15 09:45:00 by hde-barr         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

/* Handles input redirection (<) */
int	handle_redir_in(t_node_tree *node)
{
	int	fd;

	if (!node || !node->file)
		return (0);
	fd = open(node->file, O_RDONLY);
	if (fd < 0)
		return (perror("minishell: open redir in"), -1);
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("minishell: dup2 redir in");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

/* Handles output redirection (>) */
int	handle_redir_out(t_node_tree *node)
{
	int	fd;

	if (!node || !node->file)
		return (0);
	fd = open(node->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (perror("minishell: open redir out"), -1);
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2 redir out");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

/* Handles output append redirection (>>) */
int	handle_append(t_node_tree *node)
{
	int	fd;

	if (!node || !node->file)
		return (0);
	fd = open(node->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (perror("minishell: open append"), -1);
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2 append");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

/* Reads input, duplicates read end, closes pipe ends */
static int	process_heredoc_pipe(int pipefd[2], const char *delimiter)
{
	int	read_status;

	read_status = read_heredoc_input(pipefd[1], delimiter);
	close(pipefd[1]);
	if (read_status == -1)
	{
		close(pipefd[0]);
		return (-1);
	}
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
	{
		perror("minishell: dup2 heredoc");
		close(pipefd[0]);
		return (-1);
	}
	close(pipefd[0]);
	return (0);
}

/* Handles heredoc redirection (<<) */
int	handle_heredoc(t_node_tree *node)
{
	int		pipefd[2];
	char	*delimiter;

	if (!node || !node->file)
	{
		ft_putstr_fd("minishell: Heredoc node missing delimiter\n", 2);
		return (-1);
	}
	delimiter = node->file;
	if (pipe(pipefd) == -1)
		return (perror("minishell: pipe heredoc"), -1);
	return (process_heredoc_pipe(pipefd, delimiter));
}
