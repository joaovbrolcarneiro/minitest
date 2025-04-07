/* ************************************************************************** */
/* executor.c                                                                 */
/* ************************************************************************** */

#include "minishell.h"

/*
** execute_ast:
**   - Traverses the AST (built by the parser) and dispatches execution.
**   - For pipeline nodes, it calls execute_pipe_command.
**   - For redirection nodes, it sets up redirections before executing a simple command.
*/
int execute_ast(t_shell *shell, t_node_tree *node)
{
    int status = 0;
    int original_fds_saved = 0; // Flag
    int original_fds[2];        // [0]=stdin, [1]=stdout

    if (!node)
        return (0);

    // NOTE: Conditions use the NEW enum values after reordering in header
    if (node->type == AST_PIPE) // Value 0
    {
        status = execute_pipe_command(shell, node);
        // exit_status should be updated within execute_pipe_command or here based on return
        shell->exit_status = status; // Assuming execute_pipe_command returns final status
    }
    // Check for redirection types using their NEW values (1, 2, 3, 4)
    else if (node->type == AST_REDIR_IN || node->type == AST_REDIR_OUT ||
             node->type == AST_APPEND || node->type == AST_HEREDOC)
    {
        // --- Redirection Block ---
        // 1. Save FDs
        original_fds[0] = dup(STDIN_FILENO);
        original_fds[1] = dup(STDOUT_FILENO);
        if (original_fds[0] == -1 || original_fds[1] == -1) {
            perror("dup original fds for redirection"); // perror uses stderr correctly
            if (original_fds[0] != -1) close(original_fds[0]);
            shell->exit_status = 1; return (1);
        }
        original_fds_saved = 1;

        // 2. Apply redirection
        // handle_redirections internally uses perror, which writes to stderr correctly
        status = handle_redirections(node);
        if (status != 0) { // Status from handle_redirections is likely -1 on error
            if (original_fds_saved) { close(original_fds[0]); close(original_fds[1]); }
            shell->exit_status = 1; // Map internal error (-1) to exit status 1
            return (1); // Return 1 on redirection error
        }

        // 3. Execute ASSOCIATED COMMAND (left child)
        if (node->left) {
            status = execute_ast(shell, node->left); // Recursive call, status is 0-255
        } else {
            // Use ft_putstr_fd for error message to stderr
            ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
            status = 2; // Syntax error status
        }

        // 4. Restore FDs
        if (original_fds_saved) {
            restore_fds(original_fds);
            close_fds(original_fds);
        }
        shell->exit_status = status; // Update status based on command or syntax error
    }
    else if (node->type == AST_COMMAND) // Value 5 (NEW value)
    {
        // --- Simple Command Block ---
        original_fds[0] = dup(STDIN_FILENO);
        original_fds[1] = dup(STDOUT_FILENO);
        if (original_fds[0] == -1 || original_fds[1] == -1) {
            perror("dup original fds for command"); // perror uses stderr
             if (original_fds[0] != -1) close(original_fds[0]);
             shell->exit_status = 1; return (1);
        }
         original_fds_saved = 1;

        // Call simple_command (returns 0-255)
        status = execute_simple_command(shell, node);

        if (original_fds_saved) {
            restore_fds(original_fds);
            close_fds(original_fds);
        }
        shell->exit_status = status; // Update status based on command result
    }
    else // Handle unknown node types
    {
        // Original: printf(stderr, "minishell: execute_ast: Unknown node type %d\n", node->type);
        // --- CORRECTED using libft ---
        ft_putstr_fd("minishell: execute_ast: Unknown node type ", 2); // String to stderr (fd 2)
        ft_putnbr_fd(node->type, 2);                             // Integer to stderr (fd 2)
        ft_putstr_fd("\n", 2);                                   // Newline to stderr (fd 2)
        // -----------------------------
        status = 1; // Assign error status
        shell->exit_status = status;
    }
    // Make sure status returned is within 0-255 if possible
    // Though builtins/external command already handle this
    return (status);
}

/*
** execute_command:
**   - Checks if the command is a builtin (you can add more builtins here).
**   - If not, dispatches external command execution.
*/

int execute_command(t_shell *shell, char **args)
{
    if (!args || !args[0])
        return (0);
    /* Check builtins first */
    if (ft_strcmp(args[0], "cd") == 0)
        return (ft_cd(args, &shell->env));
    if (ft_strcmp(args[0], "echo") == 0)
        return (ft_echo(args));
    if (ft_strcmp(args[0], "pwd") == 0)
        return (ft_pwd());
    if (ft_strcmp(args[0], "exit") == 0)
        return (ft_exit(args, shell));
    if (ft_strcmp(args[0], "export") == 0)
        return (ft_export(args, &shell->env));

    /* If not a builtin, execute as external command */
    return (execute_external_command(shell, args));
}

/*
** execute_simple_command:
**   - Saves current STDIN/STDOUT.
**   - Sets up any redirections.
**   - Executes the command.
**   - Restores the original file descriptors.
*/
/*
** execute_simple_command: (MODIFIED - No FD Handling)
** - Executes a command (builtin or external).
** - Assumes it's only called for AST_COMMAND nodes (now value 5).
** - NO LONGER saves/restores STDIN/STDOUT.
*/
int execute_simple_command(t_shell *shell, t_node_tree *node)
{
    int status;
    char **cmd_args = NULL;

    if (!node) { /* Error handling */ return (1); }
    // Check against the NEW value for AST_COMMAND
    if (node->type != AST_COMMAND) { // AST_COMMAND is now 5
         ft_printf(stderr, "Error: execute_simple_command called with non-command node type %d\n", node->type);
         return (1);
    }

    // Determine arguments (same logic as before)
    if (node->args && node->args[0]) {
        cmd_args = node->args;
    } else if (node->content) {
        cmd_args = malloc(sizeof(char *) * 2);
        if (!cmd_args) { perror("malloc"); return (1); }
        cmd_args[0] = node->content;
        cmd_args[1] = NULL;
    } else {
         return (0); // Empty command is success
    }

    // Execute
    status = execute_command(shell, cmd_args);

    // Cleanup temp args if allocated
    if (!(node->args && node->args[0]) && node->content) {
         if (cmd_args) free(cmd_args);
    }

    // NO save_std_fds / restore_std_fds here

    return (status);
}

/*
** execute_external_command:
**   - Forks and executes an external command using execve.
*/
int execute_external_command(t_shell *shell, char **args)
{
    pid_t pid;
    int status;
    char *cmd_path;

    pid = fork();
    if (pid == 0)
    {
        handle_child_signals();
        /* If args[0] contains a '/', assume it's a path; otherwise, search in PATH */
        if (ft_strchr(args[0], '/') != NULL)
            cmd_path = ft_strdup(args[0]);
        else
            cmd_path = find_command_path(args[0], shell->env);

        if (!cmd_path)
        {
            ft_printf(RED"Command not found: %s\n"RESET, args[0]);
            exit(127);
        }
        execve(cmd_path, args, shell->env);
        perror("execve");
        exit(errno);
    }
    else if (pid < 0)
    {
        perror("fork");
        return (-1);
    }
    else
    {
        handle_parent_signals();
        waitpid(pid, &status, 0);
    }
    return (status);
}


