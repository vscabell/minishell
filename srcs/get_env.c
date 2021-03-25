/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romanbtt <marvin@student.42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 16:25:59 by romanbtt          #+#    #+#             */
/*   Updated: 2021/03/24 19:01:16 by romanbtt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void		store_key_and_value(char **value, char **key, char *str)
{
	char *addr;

	addr = ft_strrchr(str, '=');
	if (!(*key = ft_substr(str, 0, addr - str)))
		exit_msh("ft_substr: ", strerror(errno));
	if (!(*value = ft_substr(str, addr - str + 1,
	ft_strlen(str) - (addr - str))))
		exit_msh("ft_strdup: ", strerror(errno));
}

char		**get_env_value(char *key)
{
	char	**env_values;
	char	*name;
	char	*value;
	int		i;

	name = NULL;
	value = NULL;
	i = 0;
	while (g_msh.env[i])
	{
		store_key_and_value(&value, &name, g_msh.env[i]);
		if (!ft_strcmp(key, name))
		{
			if (!(env_values = ft_split(value, ':')))
				exit_msh("ft_split: ", strerror(errno));
			free(name);
			free(value);
			return (env_values);
		}
		free(name);
		free(value);
		i++;
	}
	return (NULL);
}

static void	allocate_and_atribute(char **envp)
{
	int		len_arr;
	int		i;

	len_arr = 0;
	while (envp[len_arr])
		len_arr++;
	if (!(g_msh.env = ft_calloc(len_arr + 1, sizeof(char *))))
		exit_msh("ft_calloc: ", strerror(errno));
	i = 0;
	while (i < len_arr)
	{
		if (!(g_msh.env[i] = ft_strdup(envp[i])))
			exit_msh("ft_strdup: ", strerror(errno));
		i++;
	}
}

void		init_env(char **envp)
{
	allocate_and_atribute(envp);
}
