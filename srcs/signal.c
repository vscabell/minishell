/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Vs-Rb <marvin@student.42sp.org.br>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 23:12:01 by user42            #+#    #+#             */
/*   Updated: 2021/03/22 11:24:05 by Vs-Rb            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Ctrl+C - SIGINT
// Ctrl+\ - SIGQUIT

void	signal_handler_prompt(int sig)
{
	if (sig == SIGINT)
	{
		ft_printf("^C\n");
		g_msh.last_ret_cmd = 130;
		ft_bzero(g_msh.line, ft_strlen(g_msh.line));
        print_prompt();
	}
}

void	signal_handler_do_nothing(int sig)
{
	if (sig == SIGQUIT)
		sleep(0);
}

void	signal_handler_parent(int signal)
{
	if (signal == SIGINT)
	{
		ft_printf("\n");
		g_msh.last_ret_cmd = 130;
	}
	if (signal == SIGQUIT)
	{
		ft_printf("Quit (core dumped)\n");
		g_msh.last_ret_cmd = 131;
	}
}

void	handle_signals(int caller, int pid)
{
	if (caller == ROOT)
	{
		signal(SIGINT, signal_handler_prompt);
		signal(SIGQUIT, signal_handler_do_nothing);
	}
	else if (caller == FORK)
	{
		if (pid != 0)
		{
			signal(SIGINT, signal_handler_parent);
			signal(SIGQUIT, signal_handler_parent);
		}
	}
}