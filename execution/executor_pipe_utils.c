#include "minishell.h"

void	left_child(t_shell *shell, t_node_tree *left, int pipefd[2])
{
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[0]);
	close(pipefd[1]);
	execute_ast(shell, left);
	exit(0);
}

void	right_child(t_shell *shell, t_node_tree *right, int pipefd[2])
{
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	close(pipefd[1]);
	execute_ast(shell, right);
	exit(0);
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