/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* executor_signal.c                                  :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/04/15 01:00:02 by hde-barr          #+#    #+#             */
/* Updated: 2025/04/15 10:15:00 by hde-barr         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

/* Sets signal handlers for the main shell (parent process) */
void	handle_parent_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

/* --- Heredoc functions (Should ideally be in executor_redir_utils.c) --- */

/* Helper: Checks if line matches heredoc delimiter */
static bool	is_heredoc_delim(const char *line, const char *delimiter)
{
	size_t	len;
	bool	match_name;
	bool	match_end;

	len = ft_strlen(delimiter);
	match_name = (ft_strncmp(line, delimiter, len) == 0);
	match_end = (line[len] == '\n' || line[len] == '\0');
	return (match_name && match_end);
}

/* Helper: Reads heredoc lines until delimiter or EOF */
/* Returns 0 on success, -1 on write error */
static int	read_heredoc_lines(int pipe_write_fd, const char *delimiter)
{
	char	*line;

	while (1)
	{
		ft_putstr_fd("> ", STDOUT_FILENO);
		line = get_next_line(STDIN_FILENO);
		if (line == NULL)
		{
			ft_putstr_fd("minishell: warning: here-document delimited EOF", 2);
			ft_putstr_fd(" (wanted `", 2);
			ft_putstr_fd((char *)delimiter, 2);
			ft_putstr_fd("')\n", 2);
			return (0);
		}
		if (is_heredoc_delim(line, delimiter))
		{
			free(line);
			return (0);
		}
		if (write(pipe_write_fd, line, ft_strlen(line)) == -1)
			return (perror("minishell: write heredoc pipe"), free(line), -1);
		free(line);
	}
}

/* Reads heredoc input (Split to satisfy Norminette line count) */
int	read_heredoc_input(int pipe_write_fd, const char *delimiter)
{
	return (read_heredoc_lines(pipe_write_fd, delimiter));
}

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
