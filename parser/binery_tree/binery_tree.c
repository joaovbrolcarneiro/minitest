/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binery_tree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 21:06:10 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/02 16:40:27 by hde-barr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/minishell.h"
#include "minishell_part2.h"


t_token *get_lastone_nodeof_rank(t_token *lst, t_ranking this_ranking)///////lkd_lst
{
	t_token *current_lastone;

	current_lastone = NULL;
	while (lst)
	{
		if (lst->rank == this_ranking)
			current_lastone = lst;
		if (!lst->next)
			return (current_lastone);
		lst = lst->next;
	}
	return (NULL);
}

t_token *get_prev_node(t_token *node, t_token *lst)/////////////lkb_lst
{
	if(!node || !lst)
		return (NULL);
	if(node->id == lst->id)
		return (NULL);
	while (lst)
	{
	//ft_printf("ok\n");
		if(!lst->next)
			return (NULL);
		if(lst->next->id == node->id)
			return (lst);
		lst = lst->next;
	}
	return (NULL);
}

t_token *untie_node(t_token *node, t_token *lst)///////////lkd_lst
{
	t_token *prev;

	if(!node)
		return(NULL);
	if(!node->value)
		return(NULL);
	prev = get_prev_node(node, lst);
	if(prev)
		prev->next = node->next;
	if (!prev && !node->next)
		return (NULL);
	node->next = NULL;
	if(!prev)
		return(NULL);
	return (prev->next);
}


int	ischarset(const char *set, char c)/////utils1
{
	while (*set)
	{
		if (*set == c)
			return (1);
		set++;
	}
	return (0);
}

size_t ft_strsetlen(char *s, const char *set)//////utils1
 {
	size_t	len;

	if (!s)
		return (0);
	len = 0;
	while (s[len] && !ischarset(set, s[len]))
		len++;
	return (len);
 }

size_t ft_strnlen(char *s, char n)/////utils1
 {
	size_t	len;

	if (!s)
		return (0);
	len = 0;
	while (s[len] && s[len] != n)
		len++;
	return (len);
 }

char *get_envar(char **env, char *var)///////envar0
{
	int i;

	i = -1;
	/*if (*var == '$')
		var ++;
	else
		return (NULL);*/
	while (env[++i])
	{
		if (ft_strncmp(env[i], var, ft_strnlen(env[i], '=')) == 0 \
		&& ft_strncmp(env[i], var, ft_strlen(var)) == 0)
			return (env[i] + ft_strnlen(env[i], '=') + 1);
	}
	return (NULL);
}

void print_env(char **env)/////////////envar2
{
	int i;

	i = -1;
	if(!env)
	{
		ft_printf("env not setted");
		return ;
	}
	while (env[++i])
	{
		ft_printf("%s\n", env[i]);
	}
}

char *domane_expantion(char **env, char *input)//////envar2
{
	if (ft_strncmp(ft_strtrim(input, " "),"env", ft_strlen(input) + 3) == 0)
		print_env(env);
	if(get_envar(env, input) /*&& *input == '$'*/)
		return (get_envar(env, input));
	return ("");
}

