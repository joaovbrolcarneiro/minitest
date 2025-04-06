/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbege_colector.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 17:48:57 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/03 20:18:10 by hde-barr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "libft.h"
#include <stdio.h>

size_t **garbege(void *trash)
{
    static size_t *garbege_lst[10000000];
    static size_t i = 0;

    if(trash == NULL)
    {
        return(garbege_lst[i] = NULL, i = 0, garbege_lst);
    }
    garbege_lst[i] = trash;
    i++;
    return (garbege_lst);
}

void minigarbege_colector()
{
    size_t**garbege_lst;
    size_t i = 0;

    garbege_lst = garbege(NULL);
    while (garbege_lst[i])
    {
        //printf("(freed: %p)\n", garbege_lst[i]);
        free(garbege_lst[i]);
        i++;
    }
    
}

void *hb_malloc(size_t size)
{
    void *trash;

    trash = malloc(size);
    garbege(trash);
    return (trash);
}
