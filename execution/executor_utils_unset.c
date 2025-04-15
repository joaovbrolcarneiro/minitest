/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* executor_utils_unset.c                             :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/* mplíčdeps   +#+           */
/* Created: 2025/04/15 11:55:02 by hde-barr          #+#    #+#             */
/* Updated: 2025/04/15 11:55:03 by hde-barr         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

/* Helper for ft_unset: Finds and removes one var */
static int	find_and_remove_var(t_shell *shell, const char *varname)
{
	int		j;
	int		k;
	size_t	name_len;

	j = 0;
	name_len = ft_strlen(varname);
	while (shell->env[j])
	{
		if (ft_strncmp(shell->env[j], varname, name_len) == 0 \
			&& shell->env[j][name_len] == '=')
		{
			free(shell->env[j]);
			k = j;
			while (shell->env[k + 1])
			{
				shell->env[k] = shell->env[k + 1];
				k++;
			}
			shell->env[k] = NULL;
			return (1);
		}
		j++;
	}
	return (0);
}

/* Builtin unset command */
int	ft_unset(char **args, t_shell *shell)
{
	int	i;
	int	exit_code;

	i = 1;
	exit_code = 0;
	if (!args[1])
		return (0);
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			exit_code = 1;
		}
		else
		{
			find_and_remove_var(shell, args[i]);
		}
		i++;
	}
	return (exit_code);
}
