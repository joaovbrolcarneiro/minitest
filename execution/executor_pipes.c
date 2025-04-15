/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* executor_pipes.c                                     :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/* mplíčdeps   +#+           */
/* Created: 2025/04/14 00:00:00 by hde-barr          #+#    #+#             */
/* Updated: 2025/04/14 21:10:00 by hde-barr         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

/* Creates a pipe */
int	setup_pipes(int pipefd[2])
{
	return (pipe(pipefd));
}

/* Waits for pipe children and determines final exit code from last command */
static int	wait_and_get_exit_code(pid_t pid1, pid_t pid2)
{
	int	status1;
	int	status2;
	int	exit_code;

	exit_code = 1;
	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	if (WIFEXITED(status2))
	{
		exit_code = WEXITSTATUS(status2);
	}
	else if (WIFSIGNALED(status2))
	{
		exit_code = 128 + WTERMSIG(status2);
	}
	else
	{
		exit_code = EXIT_FAILURE;
	}
	return (exit_code);
}

/* Forks left child, checks error, closes parent pipe ends on error */
static pid_t	fork_left_checked(t_shell *shell, t_node_tree *node, \
														int pipefd[2])
{
	pid_t	pid;

	pid = fork_left(shell, node, pipefd);
	if (pid < 0)
	{
		close(pipefd[0]);
		close(pipefd[1]);
	}
	return (pid);
}

/* Forks right child, checks error, closes parent pipe ends on error */
static pid_t	fork_right_checked(t_shell *shell, t_node_tree *node, \
														int pipefd[2])
{
	pid_t	pid;

	pid = fork_right(shell, node, pipefd);
	if (pid < 0)
	{
		close(pipefd[0]);
		close(pipefd[1]);
	}
	return (pid);
}

/* Executes a command pipeline (Norminette Compliant) */
int	execute_pipe_command(t_shell *shell, t_node_tree *node)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		exit_code;

	pid1 = -1;
	pid2 = -1;
	if (!node || !node->left || !node->right)
		return (2);
	if (setup_pipes(pipefd) < 0)
		return (perror("minishell: pipe"), 1);
	pid1 = fork_left_checked(shell, node, pipefd);
	if (pid1 < 0)
		return (1);
	pid2 = fork_right_checked(shell, node, pipefd);
	if (pid2 < 0)
	{
		waitpid(pid1, NULL, 0);
		return (1);
	}
	close(pipefd[0]);
	close(pipefd[1]);
	exit_code = wait_and_get_exit_code(pid1, pid2);
	return (exit_code);
}
