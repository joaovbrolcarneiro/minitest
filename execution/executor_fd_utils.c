/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* executor_fd_utils.c                                :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/* mplíčdeps   +#+           */
/* Created: 2025/04/15 01:00:00 by hde-barr          #+#    #+#             */
/* Updated: 2025/04/15 01:00:01 by hde-barr         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

/* Saves current STDIN/STDOUT using dup */
void	save_std_fds(t_shell *shell)
{
	if (!shell)
		return ;
	shell->saved_stdin = dup(STDIN_FILENO);
	shell->saved_stdout = dup(STDOUT_FILENO);
	if (shell->saved_stdin == -1 || shell->saved_stdout == -1)
	{
		perror("minishell: save_std_fds: dup failed");
		if (shell->saved_stdin != -1)
			close(shell->saved_stdin);
		if (shell->saved_stdout != -1)
			close(shell->saved_stdout);
		shell->saved_stdin = -1;
		shell->saved_stdout = -1;
	}
}

/* Restores STDIN/STDOUT from saved values and closes saved fds */
void	restore_std_fds(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->saved_stdin != -1)
	{
		if (dup2(shell->saved_stdin, STDIN_FILENO) == -1)
			perror("minishell: restore_std_fds: dup2 stdin failed");
		close(shell->saved_stdin);
		shell->saved_stdin = -1;
	}
	if (shell->saved_stdout != -1)
	{
		if (dup2(shell->saved_stdout, STDOUT_FILENO) == -1)
			perror("minishell: restore_std_fds: dup2 stdout failed");
		close(shell->saved_stdout);
		shell->saved_stdout = -1;
	}
}

/* Restores STDIN/STDOUT from a temporary array */
void	restore_fds(int fds[2])
{
	if (dup2(fds[0], STDIN_FILENO) == -1)
		perror("minishell: restore_fds: dup2 stdin failed");
	if (dup2(fds[1], STDOUT_FILENO) == -1)
		perror("minishell: restore_fds: dup2 stdout failed");
}

/* Closes file descriptors in a temporary array */
void	close_fds(int fds[2])
{
	if (fds[0] >= 0)
		close(fds[0]);
	if (fds[1] >= 0)
		close(fds[1]);
}

void	handle_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
