/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buildin_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vscabell <vscabell@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/23 01:16:54 by vscabell          #+#    #+#             */
/*   Updated: 2021/03/13 18:09:20 by vscabell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

builtin_funct	*is_builldin(char *cmd)
{
	static char				*rel_path[7] = {"echo", "cd", "pwd", "export",
		"unset", "env", "exit"};
	static char				*abs_path[7] = {"/bin/echo", "/bin/cd", "/bin/pwd",
		"/bin/export", "/bin/unset", "/bin/env", "/bin/exit"};
	static builtin_funct	f_call[7] = {ft_echo, ft_cd, ft_pwd, ft_export,
		ft_unset, ft_env, ft_exit};
	int						i;

	i = 0;
	while (i < 7)
	{
		if (!(ft_strcmp(cmd, rel_path[i])) || !(ft_strcmp(cmd, abs_path[i])))
			return (&f_call[i]);
		i++;
	}
	return (NULL);
}

int		ft_echo(t_cmd *cmd)
{
	bool	flag;
	int		i;

	flag = false;
	i = 1;
	if (cmd->args[i] && !ft_strcmp(cmd->args[i], "-n"))
	{
		flag = true;
		i++;
	}
	while (cmd->args[i])
	{
		ft_putstr_fd(cmd->args[i], STDOUT_FILENO);
		if (cmd->args[i])
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
	if (!flag)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (EXIT_SUCCESS);
}

int		ft_cd(t_cmd *cmd)
{
	if (chdir(cmd->args[1]) != 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int		ft_pwd(t_cmd *cmd)
{
	char	cwd[1024 + 1];

	(void)cmd;
	if (!(getcwd(cwd, 1024)))
		return (EXIT_FAILURE);
	ft_putendl_fd(cwd, STDOUT_FILENO);
	return (EXIT_SUCCESS);
}

int		ft_exit(t_cmd *cmd)
{
	// free_shell();
	ft_array_clear(g_env, ft_free);
	exit(EXIT_SUCCESS);
	return (EXIT_SUCCESS);
}
