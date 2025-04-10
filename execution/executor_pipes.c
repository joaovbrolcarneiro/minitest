/* ************************************************************************** */
/* executor_pipes.c                                                         */
/* ************************************************************************** */

#include "minishell.h"

/*
** setup_pipes:
**   - Creates a pipe and returns the result.
*/
int setup_pipes(int pipefd[2])
{
    return (pipe(pipefd));
}

/*
** execute_pipe_command:
**   - Executes a command pipeline.
**   - Forks one child for the left side (writing end) and one for the right side (reading end).
*/
int execute_pipe_command(t_shell *shell, t_node_tree *node)
{
    int     pipefd[2];
    pid_t   pid1 = -1, pid2 = -1;
    int     status1, status2; // Raw waitpid status
    int     exit_code = 1;    // Final 0-255 code

    if (!node || !node->left || !node->right) { /* Handle error */ return 2; }
    if (setup_pipes(pipefd) < 0) { /* Handle error */ return 1; }

    pid1 = fork_left(shell, node, pipefd);
    if (pid1 < 0) { /* Handle error */ close(pipefd[0]); close(pipefd[1]); return 1; }

    pid2 = fork_right(shell, node, pipefd);
    if (pid2 < 0) { /* Handle error */ close(pipefd[0]); close(pipefd[1]); waitpid(pid1, &status1, 0); return 1; }

    close(pipefd[0]); close(pipefd[1]); // Parent closes pipe

    waitpid(pid1, &status1, 0);
    waitpid(pid2, &status2, 0); // Wait for LAST command

    // Determine exit code from the LAST command's raw status (status2)
    if (WIFEXITED(status2)) {
        exit_code = WEXITSTATUS(status2);
    } else if (WIFSIGNALED(status2)) {
        exit_code = 128 + WTERMSIG(status2);
        // Signal printing is handled by waitpid in execute_external_command
    } else {
        exit_code = EXIT_FAILURE;
    }

    // execute_ast (the caller) will update g_exit_code
    return (exit_code); // Return interpreted code
}