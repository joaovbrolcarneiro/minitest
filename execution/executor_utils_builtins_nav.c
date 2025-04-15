/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* executor_utils_builtins_nav.c                      :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/* mplíčdeps   +#+           */
/* Created: 2025/04/15 12:15:00 by hde-barr          #+#    #+#             */
/* Updated: 2025/04/15 12:15:01 by hde-barr         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

/* Helper for get_cd_target: Handles 'cd -' */
static char	*handle_cd_dash(char **env)
{
	char	*target;

	target = get_env_value(env, "OLDPWD");
	if (!target)
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
	else
	{
		ft_putstr_fd(target, 1);
		ft_putstr_fd("\n", 1);
	}
	return (target);
}

/* Helper for ft_cd: Get target directory path */
static char	*get_cd_target(char **args, char **env)
{
	char	*target;
	char	*env_val;

	target = NULL;
	if (!args[1] || ft_strcmp(args[1], "~") == 0)
	{
		env_val = get_env_value(env, "HOME");
		if (!env_val)
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		target = env_val;
	}
	else if (ft_strcmp(args[1], "-") == 0)
	{
		target = handle_cd_dash(env);
	}
	else
		target = args[1];
	return (target);
}

/* Helper for ft_cd: Perform chdir and update PWD/OLDPWD */
static int	perform_chdir_and_update(char *target, char *prev_dir, \
										char ***env)
{
	char	*new_dir;

	if (chdir(target) != 0)
	{
		perror("minishell: cd");
		free(prev_dir);
		return (1);
	}
	update_env(env, "OLDPWD", prev_dir);
	free(prev_dir);
	new_dir = getcwd(NULL, 0);
	if (!new_dir)
		return (perror("minishell: cd: getcwd(after)"), 1);
	update_env(env, "PWD", new_dir);
	free(new_dir);
	return (0);
}

/* Builtin cd command */
int	ft_cd(char **args, char ***env)
{
	char	*prev_dir;
	char	*target;

	prev_dir = getcwd(NULL, 0);
	if (!prev_dir)
		return (perror("minishell: cd: getcwd(before)"), 1);
	target = get_cd_target(args, *env);
	if (!target)
	{
		free(prev_dir);
		return (1);
	}
	return (perform_chdir_and_update(target, prev_dir, env));
}

/* Builtin pwd command */
int	ft_pwd(void)
{
	char	cwd[4096];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		ft_putstr_fd(cwd, 1);
		ft_putstr_fd("\n", 1);
		return (0);
	}
	else
	{
		perror("minishell: pwd");
		return (1);
	}
}
