/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 13:43:12 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/01 16:14:29 by hde-barr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_code;

/*char *matrix_join(char **matrix, int size)
{
	int i;
	int size_cpy;
	char *dst;

	i = 0;
	size_cpy = size;
	while(size--)
		i += ft_strlen(matrix[size]);
	dst = ft_calloc(sizeof(char), (i + size_cpy + 1));
 	while (++size < size_cpy)
	{
		dst = ft_strjoin_gnl(dst, matrix[size]);
		dst = ft_strjoin_gnl(dst, " ");
	}
	//ft_printf("%s(%d)", dst, ft_strlen(dst));
	free(dst);
	return(NULL);
}*/

/*bool konosuclear(bool cmd_exist, const char *input)
{
	if(ft_strncmp(ft_strtrim(input, " "), "clear", 4096) == 0)
	{
		pid_t pid = fork();
		if(pid == 0)
		{
			execve("/usr/bin/clear", (char *[]){"/bin/clear", NULL}, \
			(char *[]){"TERM=xterm-256color", NULL});
		}
		else
		{
			wait(&g_exit_code);
		}
		return (true);
	}
	return (cmd_exist != false);
 	//if(ft_strncmp(ft_strtrim(input, " "), "clear", 4096) == 0)
	//	{
	//		ft_putstr_fd("\033[H\033[J", 1);
	//		return (true);
	//	}
	//return (cmd_exist != false);
}*/

/*bool konoexit(bool cmd_exist, const char *input)
{
	if(ft_strncmp(ft_strtrim(input, " "), "exit", 4096) == 0)
		{
			ft_putstr_fd("\033[H\033[J", 1);
			ft_printf(REDK"EKUSUPUROOOOSHION!💥.."GREEN"..BYEBYE (^.^)/\n");
			exit(0);
		}
	return (cmd_exist != false);
}*/

/*bool konopwd(bool cmd_exist, const char *input)
{
	char *buf;
	const size_t size = 4096;

	buf = malloc(size);
	if(ft_strncmp(ft_strtrim(input, " "), "pwd", 4096) == 0)
	{
		ft_printf(PINK"%s\n", getcwd(buf, size));
		return (true);
	}
	return (cmd_exist != false);
}*/

/*bool kazuchdir(bool cmd_exist, const char *input)
{
	if(ft_strncmp(ft_strtrim(input, " "), "cd ..", 4096) == 0)
	{
		chdir("./..");
		return (true);
	}
	return (cmd_exist != false);
}*/

/*bool kazu_ls(bool cmd_exist, const char *input)
{
	if(ft_strncmp(ft_strtrim(input, " "), "ls", 4096) == 0)
	{
		pid_t pid = fork();
		if(pid == 0)
		{
			if(access("/usr/bin/ls",F_OK) == true)
				execve("/usr/bin/ls", (char *[]){"ls", NULL}, (char *[]){"ls", NULL});
			else
				execve("/bin/ls", (char *[]){"ls", NULL}, (char *[]){"ls", NULL});
		}
		else
		{
			wait(&g_exit_code);
		}
		return (true);
	}
	return (cmd_exist != false);
}*/

/*void idontknowwhatisit(bool cmd_exist, const char *input)
{
	if(cmd_exist == false && ft_strlen(input))
		ft_printf(RED"Sorry, I don't know what is it... (T.T')\n");
}*/


/*void print_loading(void)
{
	int loop; 
	int i;
	const char *animation[] = {"🕐", "🕑⏳", "🕒⏳⏳", "🕓⏳⏳⏳", \
	"🕔⏳⏳⏳⏳", "🕕", "🕗⏳", "🕘⏳⏳", "🕚⏳⏳⏳", "🕛⏳⏳⏳⏳"};
	
	i = -1;
	loop = -1;
	while (++i < 25)
	{	
		if(loop == 9)
			loop = -1;
		ft_printf("%s", animation[++loop]);
		usleep(100000);
		ft_printf("\r\033[K");
	}
	ft_putstr_fd("\033[H\033[J", 1);
}*/

/*void print_loadingv2()!!!!!!!!!!!!!!!
{
	const char*	clock_icon="🕐🕑🕒🕓🕔🕕🕗🕘🕚🕛";
	const char*	time_icon="⏳⏳⏳";
	int i;
	int loop;

	loop = 0;
	i = 0;
	while (++loop < 5)
	{
		while (i < ft_strlen(clock_icon))
		{
			ft_printf("\r[%s]\033[K",ft_substr(clock_icon, i, 4));
			ft_printf("%s",ft_substr(time_icon, 0, i));
			usleep(100000);
			i += 4;
		}
		//ft_printf("\033[%dC", loop);
		i = 0;
	}
}*/

/*bool line_too_long(char *input)
{
	if(ft_strlen(input) > 130000)
		return (ft_printf(REDK"konosubash: line too long\n"),true);
	return (false);
}*/
