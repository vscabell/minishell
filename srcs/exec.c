/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vscabell <vscabell@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/27 15:45:12 by vscabell          #+#    #+#             */
/*   Updated: 2021/02/27 21:24:33 by vscabell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>

int	launch(char *path, char **args, char **envp)
{
	int		pid;
	int		wpid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		if (execve(path, args, envp) == -1)
			exit (EXIT_FAILURE);
		exit (EXIT_SUCCESS);
	}
	else if (pid < 0)
		exit (EXIT_FAILURE);
	else
	{
		do {
			wpid = waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	return (1);
}

char	**put_tk_lst_into_array_pointers(t_token *tks)
{
	char	**args;
	int		len;
	int		i;

	len = ft_tkn_size(tks);
	args = (char **)calloc(len + 1, sizeof(char *));
	i = 0;
	while (i < len)
	{
		args[i] = tks->data;
		tks = tks->next;
		i++;
	}
	args[i] = NULL;
	return (args);
}

int	launch_process(t_shell *sh)
{
	sh->argv = put_tk_lst_into_array_pointers(sh->tks);
	sh->path = find_bin_path(sh->env, sh->argv[0]);
	return (launch(sh->path, sh->argv, sh->envp));
}

int		execute(t_shell *sh)
{
	static int				*f_name[7] = {"echo",
										"cd",
										"pwd",
										"export",
										"unset",
										"env",
										"exit"};
	static builtin_funct	f_call[7] = {ft_echo,
										ft_cd,
										ft_pwd,
										ft_export,
										ft_unset,
										ft_env,
										ft_exit};
	int						i;

	i = 0;
	while (i < 7)
	{
		if (!(ft_strcmp(sh->tks->data, f_name[i])))
			return ((*f_call[i])(sh));
		i++;
	}
	return launch_process(sh); // to implement
}