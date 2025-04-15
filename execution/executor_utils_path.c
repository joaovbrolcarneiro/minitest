/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* executor_utils_path.c                              :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/* mplíčdeps   +#+           */
/* Created: 2025/04/15 10:40:04 by hde-barr          #+#    #+#             */
/* Updated: 2025/04/15 12:40:00 by hde-barr         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/stat.h> // Required for stat() and S_ISDIR

/* Joins a directory path and a file name with '/' */
/* Caller must free the result */
char	*ft_path_join(char *path, char *file)
{
	char	*tmp;
	char	*res;

	if (!path || !file)
		return (NULL);
	tmp = ft_strjoin(path, "/");
	if (!tmp)
		return (NULL);
	res = ft_strjoin(tmp, file);
	free(tmp);
	return (res);
}

/* Helper for find_command_path: checks access in PATH directories */
/* Returns 0 on success (path found), 1 if not found, -1 on malloc error */
static int	check_path_access(char *cmd, char **paths, char **found_path)
{
	int			i;
	char		*full_path;
	struct stat	s;

	i = 0;
	*found_path = NULL;
	while (paths && paths[i])
	{
		full_path = ft_path_join(paths[i], cmd);
		if (!full_path)
			return (-1);
		if (access(full_path, X_OK) == 0)
		{
			if (stat(full_path, &s) == 0 && !S_ISDIR(s.st_mode))
			{
				*found_path = full_path;
				return (0);
			}
		}
		free(full_path);
		i++;
	}
	return (1);
}

/* Helper for find_command_path: checks if cmd is a direct executable path */
static char	*check_direct_path(char *cmd)
{
	struct stat	s;

	if (access(cmd, X_OK) == 0)
	{
		if (stat(cmd, &s) == 0 && !S_ISDIR(s.st_mode))
			return (ft_strdup(cmd));
	}
	return (NULL);
}

/* Helper for find_command_path: searches through PATH */
static char	*search_in_path_env(char *cmd, char **env)
{
	char	*path_env;
	char	**paths;
	char	*found_path;
	int		result;

	found_path = NULL;
	path_env = get_env_value(env, "PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	result = check_path_access(cmd, paths, &found_path);
	ft_free_strarray(paths);
	if (result == 0)
		return (found_path);
	return (NULL);
}

/* Finds the full path of an executable command using PATH */
/* Returns allocated string (needs free) or NULL */
char	*find_command_path(char *cmd, char **env)
{
	char	*result_path;

	if (!cmd || cmd[0] == '\0')
		return (NULL);
	result_path = NULL;
	if (ft_strchr(cmd, '/'))
	{
		result_path = check_direct_path(cmd);
		return (result_path);
	}
	else
	{
		result_path = search_in_path_env(cmd, env);
		return (result_path);
	}
}
