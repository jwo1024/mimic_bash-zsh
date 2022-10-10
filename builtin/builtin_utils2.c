/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeyjeon <@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 17:57:21 by jaeyjeon          #+#    #+#             */
/*   Updated: 2022/10/10 20:31:47 by jaeyjeon         ###   ########.fr       */
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
	int		i;
	int		j;
	char	**new;
	char	*temp;

	i = 0;
	while (g_envp_list[i] != NULL)
		i++;
	new = malloc_env(i + 1);
	i = -1;
	while (g_envp_list[++i] != NULL)
		new[i] = ft_strdup(g_envp_list[i]);
	i = -1;
	while (new[++i] != NULL)
	{
		j = i;
		while (new[++j])
		{
			if (ft_strncmp(new[i], new[j], ft_strlen(new[i])) > 0)
			{
				temp = new[i];
				new[i] = new[j];
				new[j] = temp;
			}
		}
	}
	return (new);
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
