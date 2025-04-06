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

    if (!node)
        return (0);
    if (node->type == AST_PIPE)
        status = execute_pipe_command(shell, node);
    else if (node->type == AST_REDIR_IN ||
             node->type == AST_REDIR_OUT ||
             node->type == AST_APPEND ||
             node->type == AST_HEREDOC)
    {
        status = handle_redirections(node);
        if (status != 0)
            return (status);
        status = execute_simple_command(shell, node);
    }
    else if (node->type == AST_COMMAND)
    {
        status = execute_simple_command(shell, node);
    }
    else
    {
        /* Recursively execute left and right subtrees */
        if (node->left)
            status = execute_ast(shell, node->left);
        if (node->right)
            status = execute_ast(shell, node->right);
    }
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
int execute_simple_command(t_shell *shell, t_node_tree *node)
{
    int status;
    char **cmd_args;

    if (!node)
        return (0);
    save_std_fds(shell);
    // status = handle_redirections(node); <--- REMOVE THIS LINE
    // if (status != 0)
    //     return (status);
    /* If the node’s args field is NULL, use the content as the command */
    if (node->args)
        cmd_args = node->args;
    else
    {
        cmd_args = malloc(sizeof(char *) * 2);
        cmd_args[0] = node->content;
        cmd_args[1] = NULL;
    }
    status = execute_command(shell, cmd_args);
    restore_std_fds(shell);
    if (!node->args)
        free(cmd_args);
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


