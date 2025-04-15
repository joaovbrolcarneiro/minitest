/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* executor_utils_builtins.c                          :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/* mplíčdeps   +#+           */
/* Created: 2025/04/15 12:15:02 by hde-barr          #+#    #+#             */
/* Updated: 2025/04/15 12:30:00 by hde-barr         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

/* Builtin echo command */
int	ft_echo(char **args)
{
	int		i;
	bool	print_newline;

	i = 1;
	print_newline = true;
	while (args[i] && ft_strcmp(args[i], "-n") == 0)
	{
		print_newline = false;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
	if (print_newline)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (0);
}

/* Helper: Checks if a string represents a valid integer */
static bool	is_str_numeric(char *arg_str)
{
	int	i;

	i = 0;
	if (!arg_str)
		return (false);
	if (arg_str[i] == '+' || arg_str[i] == '-')
		i++;
	if (arg_str[i] == '\0')
		return (false);
	while (arg_str[i])
	{
		if (!ft_isdigit(arg_str[i]))
			return (false);
		i++;
	}
	return (true);
}

/* Helper for ft_exit: Parses numeric argument */
/* Returns 0 on success, sets exit_val, returns 1 on error */
static int	parse_exit_arg(char *arg_str, int *exit_val)
{
	if (!is_str_numeric(arg_str))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(arg_str, 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		*exit_val = 2;
		return (1);
	}
	*exit_val = ft_atoi(arg_str) % 256;
	if (*exit_val < 0)
		*exit_val += 256;
	return (0);
}

/* Builtin exit command */
int	ft_exit(char **args, t_shell *shell)
{
	int	exit_val;
	int	parse_status;

	exit_val = g_exit_code;
	ft_putstr_fd("exit\n", STDERR_FILENO);
	if (args[1])
	{
		parse_status = parse_exit_arg(args[1], &exit_val);
		if (parse_status != 0)
		{
			cleanup_shell(shell);
			exit(exit_val);
			if (args[2])
				ft_putstr_fd("minishell: exit: too many arguments\n", 2);
			return (1);
		}
	}
	cleanup_shell(shell);
	exit(exit_val);
}
