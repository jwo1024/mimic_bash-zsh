/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeyjeon <@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 17:57:21 by jaeyjeon          #+#    #+#             */
/*   Updated: 2022/10/16 20:40:10 by jaeyjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_env(int *fd)
{
	char	**copy_env;
	int		i;

	copy_env = sort_env();
	i = 0;
	while (copy_env[i] != NULL)
	{
		print_env_print_part(copy_env[i], fd);
		i++;
	}
	free_env(copy_env);
}

char	**sort_env(void)
{
	t_index	i;
	char	**new;
	char	*temp;

	new = malloc_env(get_envp_size() + 1);
	i.i = 0;
	i.j = 0;
	while (g_envp_list[++i.i] != NULL)
		new[i.j++] = safe_ft_strdup(g_envp_list[i.i], "env");
	new[i.i] = NULL;
	i.i = -1;
	while (new[++i.i] != NULL)
	{
		i.j = i.i;
		while (new[++i.j] != NULL)
		{
			if (ft_strncmp(new[i.i], new[i.j], ft_strlen(new[i.i])) > 0)
			{
				temp = new[i.i];
				new[i.i] = new[i.j];
				new[i.j] = temp;
			}
		}
	}
	return (new);
}

int	get_envp_size(void)
{
	int	size;

	size = 0;
	while (g_envp_list[size] != NULL)
		size++;
	return (size);
}

void	print_env_print_part(char *str, int *fd)
{
	int	i;

	i = 0;
	ft_putstr_fd("declare -x ", fd[STD_OUT]);
	while (str[i] != '\0')
	{
		write(fd[STD_OUT], &str[i], 1);
		if (str[i] == '=')
			ft_putstr_fd("\"", fd[STD_OUT]);
		i++;
	}
	if (find_equal(str))
		ft_putstr_fd("\"", fd[STD_OUT]);
	ft_putstr_fd("\n", fd[STD_OUT]);
	str[0] = -1;
}
