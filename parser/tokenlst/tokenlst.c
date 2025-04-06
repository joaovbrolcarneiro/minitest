/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenlst.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 21:06:35 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/02 16:33:36 by hde-barr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"
#include "minishell_part2.h"

size_t ft_strnlen(char *s, char n);
char *get_envar(char **env, char *var);

char **get_path_list(char **env)///////envar1
{
	char *path;
	char **path_list;

	path = get_envar(env,"PATH");
	path_list = ft_split(path, ':');
	return (path_list);
}

char **command_list_malloc(char **list, char **env)///////envar1
{
	long long i;
	DIR *dir;
	char **path_list;
	path_list = get_path_list(env);
	i = 0;
	while (*path_list)
	{
		dir = opendir(*path_list++);
		if(dir == NULL)
			continue;
		while (readdir(dir))
		{
			++i;
		}
		closedir(dir);
	}
	list = malloc((sizeof(char *) * i) + 1) ;
	return(list);
}

char **init_command_list(long long count, char **env)///////envar1
{
	char **list;
	char **path_list;
	DIR *dir;
	struct dirent *d;
	
	path_list = get_path_list(env);
	list = command_list_malloc(list, env);
	count = -1;
	while (*path_list)
	{
		dir = opendir(*path_list++);
		if(dir == NULL)
			continue;
		d = readdir(dir);
		while (d)
		{
			list[++count] = ft_strdup(d->d_name);
			d = readdir(dir);
		}
		closedir(dir);
	}
	list[++count] = NULL;
	return(list);
}

bool search_list(char *search, char **env)
{
	char **list;
	int i = 0;
	long long count;

	if(is_valid_exc(search))
		return (true);
	if (!search || !env)
		return (false);
	list = init_command_list(count, env);
	while (*(++list))
	{
		if(ft_strncmp(search, *list, 4096) == 0)
			return(true);
	}
	return(false);
}

long long get_token_id()//////////utils1
{
	static long long id = 0;

	return (id++);
}

char *print_type(t_token *lst)/////////////dev_uitls/print_token_lst
{
	if (!lst->value)
		return ("Root node");
	if (lst->type == TOKEN_PIPE)
		return ("PIPE");
	if (lst->type == TOKEN_REDIR_IN)
		return ("REDIR IN");
	if (lst->type == TOKEN_REDIR_OUT)
		return ("REDIR OUT");
	if (lst->type == TOKEN_APPEND)
		return ("APPEND");
	if (lst->type == TOKEN_HEREDOC)
		return ("HEREDOC");
	if (lst->type == TOKEN_CMD)
		return ("COMMAND");
	if (lst->type == TOKEN_WORD)
		return ("WORD");
	if (lst->type == TOKEN_EOF)
		return ("EOF");
	return (NULL);
}

void print_token_lst(t_token *lst)///////////dev_uttils/print_token_lst
{
	t_token *tmp;
	if(!lst)
		ft_printf(RED"   (list-> %s )\n"PINK, lst);
	while (lst)
	{
		printf(BLUE"( %s -> %s )\n"PINK, print_type(lst), lst->value);
		lst = lst->next;
	}
}

/*t_token	*last_node(t_token *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
	{
		lst = lst->next;
	}
	return (lst);
}

t_token *token_lst(t_token *token)
{
	static t_token *lst;

	if (lst)
		last_node(lst)->next = token;
	else 
		lst = token;*/
/*	static t_token *lst[4096];
	static int i = 0;
	
	lst[i++] = token;
	lst[i] = NULL;*/
//	return(lst);
//}
