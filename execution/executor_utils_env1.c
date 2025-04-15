/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* executor_utils_env1.c                              :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/* mplíčdeps   +#+           */
/* Created: 2025/04/15 10:40:00 by hde-barr          #+#    #+#             */
/* Updated: 2025/04/15 10:40:01 by hde-barr         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

/* Gets value of environment variable or returns NULL if not found */
char	*get_env_value(char **env, const char *name)
{
	int		i;
	size_t	len;

	if (!env || !name)
		return (NULL);
	len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}

/* Helper for update_env: Updates existing variable */
static int	update_existing_env(char ***env, char *new_var, const char *var)
{
	int		i;
	size_t	var_len;

	i = 0;
	var_len = ft_strlen(var);
	while ((*env)[i])
	{
		if (ft_strncmp((*env)[i], var, var_len) == 0 && \
			(*env)[i][var_len] == '=')
		{
			free((*env)[i]);
			(*env)[i] = new_var;
			return (1);
		}
		i++;
	}
	return (0);
}

/* Helper for update_env: Adds a new variable */
static int	add_new_env_var(char ***env, char *new_var)
{
	int		i;
	char	**new_env;

	i = 0;
	while ((*env)[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return (free(new_var), 1);
	i = 0;
	while ((*env)[i])
	{
		new_env[i] = (*env)[i];
		i++;
	}
	new_env[i] = new_var;
	new_env[i + 1] = NULL;
	free(*env);
	*env = new_env;
	return (0);
}

/* Updates existing env variable or adds a new one */
int	update_env(char ***env, char *var, char *value)
{
	char	*new_var;
	size_t	len;
	int		updated;

	len = ft_strlen(var) + ft_strlen(value) + 2;
	new_var = malloc(len);
	if (!new_var)
		return (perror("minishell: update_env malloc"), 1);
	ft_strcpy(new_var, var);
	ft_strcat(new_var, "=");
	ft_strcat(new_var, value);
	updated = update_existing_env(env, new_var, var);
	if (!updated)
	{
		if (add_new_env_var(env, new_var) != 0)
			return (1);
	}
	return (0);
}

bool	is_valid_identifier(const char *identifier)
{
	int	i;

	i = 0;
	if (!identifier || !(ft_isalpha(identifier[i]) || identifier[i] == '_'))
		return (false);
	i++;
	while (identifier[i])
	{
		if (!(ft_isalnum(identifier[i]) || identifier[i] == '_'))
			return (false);
		i++;
	}
	return (true);
}
