/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_executor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiwolee <jiwolee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/29 18:09:41 by jiwolee           #+#    #+#             */
/*   Updated: 2022/10/14 16:31:14 by jiwolee          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	msh_executor(t_tree *tree)
{
	pid_t	*pids;
	int		rtn;
	char	**env_path;

	if (tree == NULL)
		return (258);
	env_path = msh_executor_get_path(g_envp_list);
	rtn = -1;
	if (tree->top->right == NULL)
		rtn = msh_nopipe_builtin(tree);
	if (rtn == -1)
	{
		pids = msh_executor_malloc_pids(tree);
		if (pids == NULL)
			return (-1);
		if (rtn == -1)
			msh_executor_fork(tree->top, env_path, pids);
		rtn = msh_executor_wait_child(pids);
	}
	return (rtn);
}

char	**msh_executor_get_path(char **envp_list)
{
	int		i;
	char	**path;
	char	*tmp;

	i = 0;
	while (envp_list[i])
	{
		if (ft_strncmp(envp_list[i], "PATH=", 5) == 0)
		{
			path = ft_split(envp_list[i], ':');
			tmp = path[0];
			path[0] = ft_substr(path[0], 5, ft_strlen(path[0]) - 5);
			free(tmp);
			return (path);
		}
		i++;
	}
	return (NULL);
}

pid_t	*msh_executor_malloc_pids(t_tree *tree)
{
	t_node	*pipe_nd;
	int		cnt;
	pid_t	*pids;

	pipe_nd = tree->top;
	cnt = 0;
	while (pipe_nd)
	{
		if (pipe_nd->type != T_PIPE)
			return (NULL);
		pipe_nd = pipe_nd->right;
		cnt++;
	}
	pids = ft_calloc(cnt + 1, sizeof(pid_t));
	return (pids);
}
