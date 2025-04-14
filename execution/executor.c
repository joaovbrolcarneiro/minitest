/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* executor.c                                         :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/04/14 20:09:00 by hde-barr          #+#    #+#             */
/* Updated: 2025/04/14 23:00:02 by hde-barr         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

/* Part of execute_external_command: child process logic */
static void	child_exec_external(t_shell *shell, char **args)
{
	char	*cmd_path;

	cmd_path = NULL;
	handle_child_signals();
	if (args[0] && ft_strchr(args[0], '/'))
		cmd_path = ft_strdup(args[0]);
	else if (args[0])
		cmd_path = find_command_path(args[0], shell->env);
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd("\n", 2);
		exit(127);
	}
	execve(cmd_path, args, shell->env);
	perror("minishell: execve");
	free(cmd_path);
	if (errno == EACCES)
		exit(126);
	exit(EXIT_FAILURE);
}

/* Part of execute_external_command: parent process logic */
static int	parent_wait_external(pid_t pid)
{
	int	wait_status;
	int	exit_code;

	exit_code = EXIT_FAILURE;
	handle_parent_signals();
	waitpid(pid, &wait_status, 0);
	if (WIFEXITED(wait_status))
		exit_code = WEXITSTATUS(wait_status);
	else if (WIFSIGNALED(wait_status))
	{
		exit_code = 128 + WTERMSIG(wait_status);
		if (WTERMSIG(wait_status) == SIGQUIT)
			ft_putstr_fd("Quit (core dumped)\n", 2);
		else if (WTERMSIG(wait_status) == SIGINT)
			ft_putstr_fd("\n", 2);
	}
	return (exit_code);
}

/* Executes external command */
int	execute_external_command(t_shell *shell, char **args)
{
	pid_t	pid;

	if (!args || !args[0] || !args[0][0])
		return (1);
	pid = fork();
	if (pid == 0)
	{
		child_exec_external(shell, args);
	}
	else if (pid < 0)
	{
		return (perror("minishell: fork"), 1);
	}
	else if (pid > 0)
	{
		return (parent_wait_external(pid));
	}
	return (EXIT_FAILURE);
}

/* Checks and executes builtins, returns status or -1 if not a builtin */
static int	check_and_run_builtin(t_shell *shell, char **args)
{
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
	if (ft_strcmp(args[0], "unset") == 0)
		return (ft_unset(args, shell));
	return (-1);
}

/* Dispatches command execution (builtin or external) */
int	execute_command(t_shell *shell, char **args)
{
	int	builtin_status;

	if (!args || !args[0] || !args[0][0])
		return (0);
	builtin_status = check_and_run_builtin(shell, args);
	if (builtin_status != -1)
		return (builtin_status);
	return (execute_external_command(shell, args));
}
