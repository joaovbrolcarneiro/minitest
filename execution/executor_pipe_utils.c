#include "minishell.h"

void    left_child(t_shell *shell, t_node_tree *left, int pipefd[2])
{
    int exit_status;
    close(pipefd[0]);
    if (dup2(pipefd[1], STDOUT_FILENO) == -1) { perror("minishell: dup2 left"); exit(EXIT_FAILURE); }
    close(pipefd[1]);
    exit_status = execute_ast(shell, left);
    exit(exit_status); // Exit with actual status
}

void    right_child(t_shell *shell, t_node_tree *right, int pipefd[2])
{
    int exit_status;
    close(pipefd[1]);
    if (dup2(pipefd[0], STDIN_FILENO) == -1) { perror("minishell: dup2 right"); exit(EXIT_FAILURE); }
    close(pipefd[0]);
    exit_status = execute_ast(shell, right);
    exit(exit_status); // Exit with actual status
}

int	fork_left(t_shell *shell, t_node_tree *node, int pipefd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		left_child(shell, node->left, pipefd);
	else if (pid < 0)
	{
		perror("fork");
		return (-1);
	}
	return (pid);
}

int	fork_right(t_shell *shell, t_node_tree *node, int pipefd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		right_child(shell, node->right, pipefd);
	else if (pid < 0)
	{
		perror("fork");
		return (-1);
	}
	return (pid);
}