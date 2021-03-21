/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 23:12:01 by user42            #+#    #+#             */
/*   Updated: 2021/03/21 15:30:33 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Ctrl+C - SIGINT
// Ctrl+\ - SIGQUIT

// void	signal_handler(int sig)
// {
// 	if (sig == SIGQUIT)
// 	{
// 		ft_printf("^\\Quit (core dumped)\n");
// 		ft_bzero(g_msh.rd_line, ft_strlen(g_msh.rd_line));
// 		print_prompt();
// 	}
// }

void	signal_handler_prompt(int sig)
{
	if (sig == SIGINT)
	{
        ft_printf("^C\n");
        ft_bzero(g_msh.rd_line, ft_strlen(g_msh.rd_line));
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
		ft_printf("^C\n");
	if (signal == SIGQUIT)
		ft_printf("^\\Quit (core dumped)\n");
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
		if (pid == 0)
		{
			// o que tratar?
			// signal(SIGINT, signal_handler);
			// signal(SIGQUIT, signal_handler);
		}
		else
		{
			signal(SIGINT, signal_handler_parent);
			signal(SIGQUIT, signal_handler_parent);
		}
	}
}