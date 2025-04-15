/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* executor_utils_env2.c                              :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/* mplíčdeps   +#+           */
/* Created: 2025/04/15 10:40:02 by hde-barr          #+#    #+#             */
/* Updated: 2025/04/15 11:55:01 by hde-barr         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

/* Helper: Validates and processes a single argument for export */
static int	handle_single_export_arg(char *arg, char ***env)
{
	char	*equal;
	char	*var_name;
	int		update_status;

	equal = ft_strchr(arg, '=');
	var_name = arg;
	if (equal)
		*equal = '\0';
	if (!is_valid_identifier(var_name))
	{
		if (equal)
			*equal = '=';
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (1);
	}
	else if (equal)
	{
		update_status = update_env(env, var_name, equal + 1);
		*equal = '=';
		if (update_status != 0)
			return (1);
	}
	return (0);
}

/* Helper for ft_export: Processes arguments */
static int	process_export_args(char **args, char ***env)
{
	int	i;
	int	overall_exit_code;
	int	arg_status;

	i = 1;
	overall_exit_code = 0;
	while (args[i])
	{
		arg_status = handle_single_export_arg(args[i], env);
		if (arg_status != 0)
			overall_exit_code = arg_status;
		i++;
	}
	return (overall_exit_code);
}

/* Helper for ft_export: Prints sorted env */
static int	print_sorted_env(char ***env)
{
	char	**sorted;
	int		i;

	sorted = sort_env(*env);
	if (!sorted)
		return (perror("minishell: export"), 1);
	i = 0;
	while (sorted[i])
	{
		ft_putstr_fd(sorted[i], 1);
		ft_putstr_fd("\n", 1);
		free(sorted[i]);
		i++;
	}
	free(sorted);
	return (0);
}

/* Builtin export command */
int	ft_export(char **args, char ***env)
{
	if (!args[1])
		return (print_sorted_env(env));
	else
		return (process_export_args(args, env));
}

char	*ft_strcat(char *dest, const char *src)
{
	char	*tmp;

	tmp = dest;
	while (*dest)
	{
		dest++;
	}
	while (*src)
	{
		*dest = *src;
		dest++;
		src++;
	}
	*dest = '\0';
	return (tmp);
}
