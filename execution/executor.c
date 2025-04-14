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
    int internal_status = 0;
    int original_fds_saved = 0;
    int original_fds[2];

    if (!node) {
        g_exit_code = 0; return 0;
    }

    // --- Process based on node type ---

    if (node->type == AST_PIPE) { // Value 0
        status = execute_pipe_command(shell, node);
    }
    else if (node->type >= AST_REDIR_IN && node->type <= AST_HEREDOC) { // Values 1, 2, 3, 4
        // --- Redirection Block ---
        // (Keep the logic from response #31 - save FDs, handle_redirections, execute child, restore FDs)
        // ... (previous code for redirection block) ...
         original_fds[0] = dup(STDIN_FILENO);
         original_fds[1] = dup(STDOUT_FILENO);
         if (original_fds[0] == -1 || original_fds[1] == -1) { /* perror, set status=1 */ status = 1; goto end_ast_redir; }
         original_fds_saved = 1;
         internal_status = handle_redirections(node);
         if (internal_status != 0) { status = 1; goto end_ast_redir; }
         t_node_tree *cmd_node = node->left;
         if (!cmd_node || cmd_node->type != AST_COMMAND) {
             if (node->right && node->right->type == AST_COMMAND) { cmd_node = node->right;}
             else { cmd_node = NULL; }
         }
         if (cmd_node) { status = execute_ast(shell, cmd_node); }
         else { ft_putstr_fd("minishell: syntax error near redirection\n", 2); status = 2; }
     end_ast_redir:
         if (original_fds_saved) { restore_fds(original_fds); close_fds(original_fds); }
         // status already set above
    }
    else if (node->type == AST_COMMAND) { // Value 5
        // --- Simple Command Block ---
         original_fds[0] = dup(STDIN_FILENO);
         original_fds[1] = dup(STDOUT_FILENO);
         if (original_fds[0] == -1 || original_fds[1] == -1) { /* perror, set status=1 */ status = 1; goto end_ast_command; }
         original_fds_saved = 1;
         status = execute_simple_command(shell, node);
     end_ast_command:
         if (original_fds_saved) { restore_fds(original_fds); close_fds(original_fds); }
         // status already set above
    }
    // *** NEW: Handle pure assignment statement that reaches executor ***
    // We check against the *token* type value because the AST type is invalid
    else if (node->type == (t_ast_type)TOKEN_ASSIGNMENT) // Check if type is 9
    {
        // Assignment was already handled by process_variable_assignments.
        // If it's the only thing, command succeeds with status 0.
        status = 0;
    }
    // *******************************************************************
    // Handle TOKEN_WORD type (Value 6) - command not found
    else if (node->type == TOKEN_WORD) {
        ft_putstr_fd("minishell: command not found: ", 2);
        ft_putstr_fd(node->content ? node->content : "unknown", 2);
        ft_putstr_fd("\n", 2);
        status = 127;
    }
    else { // Truly unknown type
        ft_putstr_fd("minishell: execute_ast: Unknown node type ", 2);
        ft_putnbr_fd(node->type, 2);
        ft_putstr_fd("\n", 2);
        status = 1;
    }

    // Update global exit code at the very end
    g_exit_code = status;
    return (status);
}


/*
** execute_command:
**   - Checks if the command is a builtin (you can add more builtins here).
**   - If not, dispatches external command execution.
*/

int execute_command(t_shell *shell, char **args)
{
    // Handle empty command lines or NULL args array
    if (!args || !args[0] || !args[0][0]) // Check also if args[0] is empty string
        return (0); // Empty command is success (like Bash)

    /* Check builtins first */
    if (ft_strcmp(args[0], "cd") == 0)
        return (ft_cd(args, &shell->env)); // Assuming ft_cd returns 0/1
    if (ft_strcmp(args[0], "echo") == 0)
        return (ft_echo(args)); // Returns 0
    if (ft_strcmp(args[0], "pwd") == 0)
        return (ft_pwd()); // Returns 0/1
    if (ft_strcmp(args[0], "exit") == 0)
        return (ft_exit(args, shell)); // Exits or returns 1
    if (ft_strcmp(args[0], "export") == 0)
        return (ft_export(args, &shell->env)); // Returns 0/1
    // *** ADDED CHECK FOR UNSET ***
    if (ft_strcmp(args[0], "unset") == 0)
        return (ft_unset(args, shell)); // Returns 0/1
    // *****************************
    // Add checks for other builtins like 'env' here if you implement them

    /* If not a builtin, execute as external command */
    // execute_external_command should return the interpreted exit code (0-255)
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

    if (!node) {
        ft_putstr_fd("minishell: Error: execute_simple_command called with NULL node\n", 2);
        return (1);
    }

    // Check node type (assuming AST_COMMAND is 5 based on previous context)
    if (node->type != AST_COMMAND) {
        // --- CORRECTED Print using libft ---
        ft_putstr_fd("minishell: Error: execute_simple_command called with non-command node type ", 2);
        ft_putnbr_fd(node->type, 2);
        ft_putstr_fd("\n", 2);
        // -----------------------------------
        return (1);
    }

    // Determine arguments
    if (node->args && node->args[0]) {
        cmd_args = node->args;
    } else if (node->content) {
        cmd_args = malloc(sizeof(char *) * 2);
        if (!cmd_args) { perror("minishell: malloc"); return (1); }
        cmd_args[0] = node->content;
        cmd_args[1] = NULL;
    } else {
         return (0); // Empty command
    }

    // Execute
    status = execute_command(shell, cmd_args); // Returns 0-255

    // Cleanup temp args if allocated
    if (!(node->args && node->args[0]) && node->content) {
         if (cmd_args) free(cmd_args);
    }

    return (status);
}

/*
** execute_external_command:
**   - Forks and executes an external command using execve.
*/
int execute_external_command(t_shell *shell, char **args)
{
    pid_t   pid;
    int     wait_status;
    int     exit_code;
    char    *cmd_path = NULL;

    if (!args || !args[0]) return (1);

    pid = fork();
    if (pid == 0) // Child
    {
        handle_child_signals();
        if (args[0] && ft_strchr(args[0], '/'))
            cmd_path = ft_strdup(args[0]);
        else if (args[0])
            cmd_path = find_command_path(args[0], shell->env);

        if (!cmd_path) {
            ft_putstr_fd("minishell: command not found: ", 2);
            ft_putstr_fd(args[0] ? args[0] : "", 2);
            ft_putstr_fd("\n", 2);
            exit(127);
        }

        execve(cmd_path, args, shell->env);
        // If execve returns, it's an error
        perror("minishell: execve");
        // *** FREE cmd_path BEFORE EXITING ON ERROR ***
        free(cmd_path);
        // *********************************************
        if (errno == EACCES) exit(126);
        exit(EXIT_FAILURE); // General failure (usually 1)
    }
    else if (pid < 0) // Fork Error
    {
        perror("minishell: fork");
        // Free cmd_path if allocated in parent? No, path finding is in child.
        return (1);
    }
    else // Parent
    {
        handle_parent_signals();
        waitpid(pid, &wait_status, 0);

        if (WIFEXITED(wait_status)) { exit_code = WEXITSTATUS(wait_status); }
        else if (WIFSIGNALED(wait_status)) { exit_code = 128 + WTERMSIG(wait_status); /* Signal msg printing? */ }
        else { exit_code = EXIT_FAILURE; }
        return (exit_code);
    }
}


