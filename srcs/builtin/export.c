/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romanbtt <marvin@student.42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/04 09:45:23 by romanbtt          #+#    #+#             */
/*   Updated: 2021/03/24 20:37:55 by romanbtt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void		ft_sort_string_arr(char **arr)
{
	int		i;
	int		j;
	char	*aux;

	if (!arr || !(*arr))
		return ;
	i = 0;
	while (arr[i + 1])
	{
		j = i + 1;
		while (arr[j])
		{
			if (ft_strncmp(arr[i], arr[j], ft_strlen(arr[i])) > 0)
			{
				aux = arr[i];
				arr[i] = arr[j];
				arr[j] = aux;
			}
			j++;
		}
		i++;
	}
}

static char		**duplicate_array(char **buffer, int len_arr)
{
	char	**new_buffer;
	int		i;

	if (!buffer || !(*buffer))
		return (NULL);
	if (!(new_buffer = ft_calloc(len_arr + 1, sizeof(char *))))
		exit_msh("ft_calloc: ", strerror(errno));
	i = 0;
	while (i < len_arr)
	{
		new_buffer[i] = buffer[i];
		i++;
	}
	return (new_buffer);
}

static char		**reallocate_array(char **buffer, char *new_string)
{
	char	**new_buffer;
	int		len_arr;

	len_arr = 0;
	while (buffer[len_arr])
		len_arr++;
	new_buffer = duplicate_array(buffer, len_arr + 1);
	if (!(new_buffer[len_arr] = ft_strdup(new_string)))
		exit_msh("ft_strdup: ", strerror(errno));
	free(buffer);
	return (new_buffer);
}

static void		print_env_declare_mode(void)
{
	char	**sorted_env;
	int		i;

	i = 0;
	while (g_msh.env[i])
		i++;
	sorted_env = duplicate_array(g_msh.env, i);
	ft_sort_string_arr(sorted_env);
	i = 0;
	while (sorted_env[i])
		ft_printf("declare -x %s\n", sorted_env[i++]);
	free(sorted_env);
}

void			look_for_env(char *env_str)
{
	bool	exist;
	int		len;
	int		i;

	if ((len = ft_strrchr(env_str, '=') - env_str) < 0)
		return ;
	i = 0;
	exist = false;
	while (g_msh.env[i])
	{
		if (!ft_strncmp(g_msh.env[i], env_str, len + 1))
		{
			free(g_msh.env[i]);
			if (!(g_msh.env[i] = ft_strdup(env_str)))
				exit_msh("ft_strdup: ", strerror(errno));
			exist = true;
		}
		i++;
	}
	if (exist == false)
		g_msh.env = reallocate_array(g_msh.env, env_str);
}

void			ft_export(t_cmd *cmd, t_exec *exec)
{
	int		i;

	i = 1;
	g_msh.force_ret_buildin = true;
	g_msh.last_ret_cmd = EXIT_SUCCESS;
	if (exec->child_pid == 0)
	{
		free_after_fork();
		exit(g_msh.last_ret_cmd);
	}
	if (!cmd->args[i])
		print_env_declare_mode();
	else
	{
		while (cmd->args[i])
		{
			look_for_env(cmd->args[i]);
			i++;
		}
	}
}
