/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* executor_utils_env2.c                              :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/04/15 10:40:02 by hde-barr          #+#    #+#             */
/* Updated: 2025/04/15 11:45:00 by hde-barr         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

/* Helper for bubble_sort_env: Swaps two elements if needed */
static void	swap_if_needed(char **array, int j, bool *swapped)
{
	char	*temp;

	if (ft_strcmp(array[j], array[j + 1]) > 0)
	{
		temp = array[j];
		array[j] = array[j + 1];
		array[j + 1] = temp;
		*swapped = true;
	}
}

/* Helper for sort_env: Sorts the array using bubble sort */
static void	bubble_sort_env(char **array, int count)
{
	int		i;
	int		j;
	bool	swapped;

	if (count < 2)
		return ;
	i = 0;
	while (i < count - 1)
	{
		swapped = false;
		j = 0;
		while (j < count - i - 1)
		{
			swap_if_needed(array, j, &swapped);
			j++;
		}
		if (!swapped)
			break ;
		i++;
	}
}

/* Helper for sort_env: Duplicates the source array into dest */
static int	duplicate_env_array(char **dest, char **src, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		dest[i] = ft_strdup(src[i]);
		if (!dest[i])
			return (ft_free_strarray(dest), 1);
		i++;
	}
	dest[count] = NULL;
	return (0);
}

/* Creates a sorted duplicate of the environment array */
char	**sort_env(char **env)
{
	int		count;
	char	**sorted;

	count = 0;
	if (!env)
		return (NULL);
	while (env[count])
		count++;
	sorted = malloc(sizeof(char *) * (count + 1));
	if (!sorted)
		return (perror("minishell: sort_env malloc"), NULL);
	if (duplicate_env_array(sorted, env, count) != 0)
		return (perror("minishell: sort_env strdup"), NULL);
	bubble_sort_env(sorted, count);
	return (sorted);
}
