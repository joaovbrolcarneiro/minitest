/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* executor_signal.c                                  :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/* mplíčdeps   +#+           */
/* Created: 2025/04/15 01:00:02 by hde-barr          #+#    #+#             */
/* Updated: 2025/04/15 01:00:03 by hde-barr         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

/* Sets default signal handlers for child processes */
void	handle_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

/* Sets signal handlers for the main shell (parent process) */
void	handle_parent_signals(void)
{
	signal(SIGINT, SIG_IGN); // TODO: Replace SIG_IGN with actual handler later
	signal(SIGQUIT, SIG_IGN);
}