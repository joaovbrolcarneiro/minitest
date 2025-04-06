/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 14:03:00 by hde-barr          #+#    #+#             */
/*   Updated: 2024/12/10 15:50:22 by hde-barr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>

void	*ft_calloc_gnl(size_t nmemb, size_t size);
char	*ft_take_date(int fd, char *buffer);
char	*get_next_line(int fd);
char	*ft_strchr_gnl(char *s, int c);
int		strlen_gnl(const char *str);
char	*ft_strjoin_gnl(char *s1, char *s2);
char	*ft_take_line(char *buffer);
char	*ft_rebuffer(char *buffer);

#endif
