/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* executor_redirections.c                          :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/* mplíčdeps   +#+           */
/* Created: 2025/04/14 00:00:00 by hde-barr          #+#    #+#             */
/* Updated: 2025/04/15 00:25:00 by hde-barr         ###   ########.fr       */
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

/* Reads heredoc input until delimiter */
static int	read_heredoc_input(int pipe_write_fd, const char *delimiter)
{
	char	*line;
	size_t	delimiter_len;

	delimiter_len = ft_strlen(delimiter);
	while (1)
	{
		ft_putstr_fd("> ", STDOUT_FILENO);
		line = get_next_line(STDIN_FILENO);
		if (line == NULL)
		{
			ft_putstr_fd("minishell: warning: here-document delimited by EOF (wanted `", 2);
			// *** ADD CAST here ***
			ft_putstr_fd((char *)delimiter, 2); // Cast to char*
			ft_putstr_fd("')\n", 2);
			break ;
		}
		if (ft_strncmp(line, delimiter, delimiter_len) == 0 && \
			(line[delimiter_len] == '\n' || line[delimiter_len] == '\0'))
		{
			free(line);
			break ;
		}
		if (write(pipe_write_fd, line, ft_strlen(line)) == -1)
			return (perror("minishell: write heredoc pipe"), free(line), -1);
		free(line);
	}
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
	if (read_heredoc_input(pipefd[1], delimiter) == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	close(pipefd[1]);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
	{
		perror("minishell: dup2 heredoc");
		close(pipefd[0]);
		return (-1);
	}
	close(pipefd[0]);
	return (0);
}

/* Dispatches to the correct redirection handler */
int	handle_redirections(t_node_tree *node)
{
	if (!node)
		return (0);
	if (node->type == AST_REDIR_IN)
		return (handle_redir_in(node));
	else if (node->type == AST_REDIR_OUT)
		return (handle_redir_out(node));
	else if (node->type == AST_APPEND)
		return (handle_append(node));
	else if (node->type == AST_HEREDOC)
		return (handle_heredoc(node));
	return (0);
}