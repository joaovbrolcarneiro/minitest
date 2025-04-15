/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* executor_utils_shell.c                             :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/* mplíčdeps   +#+           */
/* Created: 2025/04/15 10:40:06 by hde-barr          #+#    #+#             */
/* Updated: 2025/04/15 10:40:07 by hde-barr         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

/* Duplicates a string array */
/* Caller must free using ft_free_strarray */
char	**ft_strdup_array(char **array)
{
	int		i;
	int		len;
	char	**dup;

	len = 0;
	if (!array)
		return (NULL);
	while (array[len])
		len++;
	dup = malloc(sizeof(char *) * (len + 1));
	if (!dup)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dup[i] = ft_strdup(array[i]);
		if (!dup[i])
			return (ft_free_strarray(dup), NULL);
		i++;
	}
	dup[len] = NULL;
	return (dup);
}

/* Frees a string array */
void	ft_free_strarray(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

/* Initializes the shell structure */
void	init_shell(t_shell *shell, char **env)
{
	if (!shell)
		return ;
	shell->env = ft_strdup_array(env);
	shell->saved_stdin = dup(STDIN_FILENO);
	shell->saved_stdout = dup(STDOUT_FILENO);
	if (shell->saved_stdin == -1 || shell->saved_stdout == -1 || !shell->env)
	{
		perror("minishell: init_shell resource allocation failed");
		if (shell->saved_stdin != -1)
			close(shell->saved_stdin);
		if (shell->saved_stdout != -1)
			close(shell->saved_stdout);
		if (shell->env)
			ft_free_strarray(shell->env);
		exit(EXIT_FAILURE);
	}
	shell->heredoc_fd = -1;
	shell->in_heredoc = 0;
	shell->ast_root = NULL;
}

/* Cleans up shell resources */
void	cleanup_shell(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->env)
	{
		ft_free_strarray(shell->env);
		shell->env = NULL;
	}
	if (shell->saved_stdin >= 0)
	{
		close(shell->saved_stdin);
		shell->saved_stdin = -1;
	}
	if (shell->saved_stdout >= 0)
	{
		close(shell->saved_stdout);
		shell->saved_stdout = -1;
	}
}
