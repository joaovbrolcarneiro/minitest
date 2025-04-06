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
int	execute_pipe_command(t_shell *shell, t_node_tree *node)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status1;
	int		status2;

	if (setup_pipes(pipefd) < 0)
	{
		perror("pipe");
		return (-1);
	}
	pid1 = fork_left(shell, node, pipefd);
	if (pid1 < 0)
		return (-1);
	pid2 = fork_right(shell, node, pipefd);
	if (pid2 < 0)
		return (-1);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	return (status1);
}