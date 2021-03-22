/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Vs-Rb <marvin@student.42sp.org.br>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/10 12:03:35 by romanbtt          #+#    #+#             */
/*   Updated: 2021/03/22 11:24:14 by Vs-Rb            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	restore_terminal()
{
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, g_msh.save) == -1)
		exit (0); // Call exit function faillure
	free(g_msh.save);
}

void update_minishell()
{
	restore_terminal();
}

int main(__attribute__((unused))int argc, __attribute__((unused))char *argv[],
	char *envp[])
{
	init_env(envp);
	print_prompt();
	while ((read_line()) != 0)
	{
		update_minishell();
		lexer();
		if (!syntax_parser())
		{
			print_prompt();
			continue;
		}
		create_commands();
		if (!find_path())
			continue;
		execution_commands();
		free(g_msh.line);
		print_prompt();
	}
	// free everything before quit.
	// return (msh.last_ret_cmd);
	// exit_program();
}
