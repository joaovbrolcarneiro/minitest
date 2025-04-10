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
    int     pipefd[2];
    char    *line;
    char    *delimiter;
    size_t  delimiter_len;
    int     gnl_ret; // To store get_next_line return value if needed

    if (!node || !node->file) {
        ft_putstr_fd("minishell: Heredoc node missing delimiter\n", STDERR_FILENO);
        return (-1); // Indicate error
    }
    delimiter = node->file;
    delimiter_len = ft_strlen(delimiter);

    // 1. Create the pipe
    if (pipe(pipefd) == -1) {
        perror("minishell: pipe for heredoc");
        return (-1);
    }

    // 2. Read input lines until delimiter is matched
    while (1) {
        // Use readline for better user experience, or get_next_line for simplicity
        // Using get_next_line for this example:
        ft_putstr_fd("> ", STDOUT_FILENO); // Print prompt (optional)
        line = get_next_line(STDIN_FILENO); // Read from standard input

        // Check for EOF (Ctrl+D)
        if (line == NULL) {
            // Bash prints a warning here, you can add it too
            ft_putstr_fd("minishell: warning: here-document delimited by end-of-file (wanted `", STDERR_FILENO);
            ft_putstr_fd(delimiter, STDERR_FILENO);
            ft_putstr_fd("')\n", STDERR_FILENO);
            break; // End loop on EOF
        }

        // Check if the line matches the delimiter (excluding potential newline)
        if (ft_strncmp(line, delimiter, delimiter_len) == 0 &&
            (line[delimiter_len] == '\n' || line[delimiter_len] == '\0'))
        {
            free(line); // Free the delimiter line
            break; // Delimiter found, end loop
        }

        // TODO: Add heredoc expansion ($VAR) here if needed (and delimiter wasn't quoted)

        // Write the line (including newline) to the pipe's write end
        if (write(pipefd[1], line, ft_strlen(line)) == -1) {
            perror("minishell: write to heredoc pipe");
            free(line);
            close(pipefd[0]); // Close pipe ends on error
            close(pipefd[1]);
            return (-1);
        }
        free(line); // Free the line after writing
    }

    // 3. Close the write end of the pipe (signals EOF to reader)
    close(pipefd[1]);

    // 4. Redirect standard input to the read end of the pipe
    if (dup2(pipefd[0], STDIN_FILENO) == -1) {
        perror("minishell: dup2 stdin for heredoc");
        close(pipefd[0]); // Close remaining pipe end on error
        return (-1);
    }

    // 5. Close the original read end (no longer needed after dup2)
    close(pipefd[0]);

    return (0); // Success
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
