/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vscabell <vscabell@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/27 15:45:12 by vscabell          #+#    #+#             */
/*   Updated: 2021/03/06 03:47:43 by vscabell         ###   ########.fr       */
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

char	**get_env_path(void)
{
	char	*name;
	char	*value;
	int		i;

	name = NULL;
	value = NULL;
	while (g_env[i])
	{
		store_value_and_name(&value, &name, i);
		if (!ft_strcmp("PATH", name))
			return (ft_split(value, ':'));
		i++;
	}
	return (NULL);
}

char	*join_path(char *env, char *path)
{
	char	*tmp;

	tmp = ft_strjoin(env, "/");
	tmp = ft_strjoin(tmp, path);
	return (tmp);
}

void	set_redirection(t_cmd *cmd)
{
	int	fd;

	if (cmd->redirection == LESSER)
	{
		fd = open(cmd->file_in, O_RDONLY);
		dup2(fd, STDIN_FILENO);
	}
	else if (cmd->redirection == GREATER)
	{
		fd = open(cmd->file_out, O_WRONLY | O_CREAT, 0664);
		dup2(fd, STDOUT_FILENO);
	}
	else if (cmd->redirection == GGREATER)
	{
		fd = open(cmd->file_out, O_WRONLY | O_CREAT | O_APPEND, 0664);
		dup2(fd, STDOUT_FILENO);
	}
}

int	launch_relative_path(t_exec *exec, t_cmd *cmd)
{
	char	**env_path;
	char	*tmp;
	int		pid;
	int		i;

	i = 0;
	env_path = get_env_path();
	pid = fork();
	if (pid == 0)
	{
		while (env_path[i])
		{
			tmp = join_path(env_path[i], exec->path);
			execve(tmp, exec->argv, g_env);
			i++;
		}
	}
	else
		wait(0);
	return (0);
}

int	launch_absolute_path(t_exec *exec)
{
	char	*tmp;
	int		pid;

	pid = fork();
	if (pid == 0)
	{
		execve(exec->path, exec->argv, g_env);
	}
	else
		wait(0);
	return (0);
}

int		execute_single_command(t_shell *sh, t_cmd *cmd)
{
	t_exec			exec;
	builtin_funct	*f_buildin;
	int				fd[2];

	f_buildin = NULL;
	exec.argv = cmd->args;
	exec.path = cmd->cmd;
	fd[0] = dup(STDIN_FILENO);
	fd[1] = dup(STDOUT_FILENO);
	if (cmd->file_in || cmd->file_out)
		set_redirection(cmd);
	if (f_buildin = is_builldin(cmd->cmd))
		(*f_buildin)(cmd);
	else if (ft_strchr("./~", cmd->cmd[0]))
		launch_absolute_path(&exec);
	else
		launch_relative_path(&exec, cmd);
	dup2(fd[0], 0);
	dup2(fd[1], 1);
	return (0);
}

int		execute(t_shell *sh)
{
	t_cmd *tmp;

	tmp = sh->cmd;
	while (tmp)
	{
		execute_single_command(sh, tmp);
		tmp = tmp->next;
	}
}
