/* ************************************************************************** */
/* executor_redirections.c                                                  */
/* ************************************************************************** */

#include "minishell.h"

/*
** handle_redir_in:
**   - Opens the file for input redirection and duplicates it to STDIN.
*/
int handle_redir_in(t_node_tree *node)
{
    int fd;

    if (!node || !node->file)
        return (0);
    fd = open(node->file, O_RDONLY);
    if (fd < 0)
    {
        perror("open redir in");
        return (-1);
    }
    dup2(fd, STDIN_FILENO);
    close(fd);
    return (0);
}

/*
** handle_redir_out:
**   - Opens/creates the file for output redirection (overwrite mode) and duplicates it to STDOUT.
*/
int handle_redir_out(t_node_tree *node)
{
    int fd;

    if (!node || !node->file)
        return (0);
    fd = open(node->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
    {
        perror("open redir out");
        return (-1);
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
    return (0);
}

/*
** handle_append:
**   - Opens/creates the file for output redirection in append mode and duplicates it to STDOUT.
*/
int handle_append(t_node_tree *node)
{
    int fd;

    if (!node || !node->file)
        return (0);
    fd = open(node->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd < 0)
    {
        perror("open append");
        return (-1);
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
    return (0);
}

/*
** handle_heredoc:
**   - A basic heredoc implementation.
**   - (In a full implementation, you would capture input until a delimiter is found.)
*/
int handle_heredoc(t_node_tree *node)
{
    (void)node;
    ft_printf("Heredoc is not fully implemented yet.\n");
    return (0);
}

/*
** handle_redirections:
**   - Chooses the proper redirection function based on the node type.
*/
int handle_redirections(t_node_tree *node)
{
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

/*
** save_std_fds & restore_std_fds:
**   - Save the current STDIN/STDOUT and restore them after command execution.
*/
void save_std_fds(t_shell *shell)
{
    shell->saved_stdin = dup(STDIN_FILENO);
    shell->saved_stdout = dup(STDOUT_FILENO);
}

void restore_std_fds(t_shell *shell)
{
    dup2(shell->saved_stdin, STDIN_FILENO);
    dup2(shell->saved_stdout, STDOUT_FILENO);
    close(shell->saved_stdin);
    close(shell->saved_stdout);
}

/*
** Signals for child and parent processes.
*/
void handle_child_signals(void)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
}

void handle_parent_signals(void)
{
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}

/*
** restore_fds and close_fds (if needed by more complex setups)
*/
void restore_fds(int fds[2])
{
    dup2(fds[0], STDIN_FILENO);
    dup2(fds[1], STDOUT_FILENO);
}

void close_fds(int fds[2])
{
    close(fds[0]);
    close(fds[1]);
}
