/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_executor_fork_wait.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeyjeon <@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 12:43:26 by jiwolee           #+#    #+#             */
/*   Updated: 2022/10/17 01:15:38 by jaeyjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "mini_signal.h"

pid_t	*msh_executor_fork(t_node *pipe_nd, char **env_path, pid_t *pids)
{
	int	i;
	int	pipe_fd[2];
	int	*fd;

	i = 0;
	if (msh_init_fd(&fd) == -1)
		return (NULL);
	while (pipe_nd)
	{
		msh_executor_fork_set_pipe1(pipe_nd, pipe_fd, fd);
		pids[i] = fork();
		if (pids[i] == -1)
			msh_print_errno(STD_ERROR, "fail executor_fork", NULL, 1);
		if (pids[i++] == 0)
		{
			if (pipe_nd->right)
				close(pipe_fd[PIPE_OUT]);
			exit(msh_run_cmd(pipe_nd->left, fd, env_path));
		}
		msh_executor_fork_set_pipe2(pipe_nd, pipe_fd, fd);
		pipe_nd = pipe_nd->right;
	}
	free (fd);
	return (pids);
}

void	msh_executor_fork_set_pipe1(t_node *pipe_nd, int *pipe_fd, int *fd)
{
	if (pipe_nd->right)
	{
		if (pipe(pipe_fd) == -1)
		{
			msh_print_errno(STD_ERROR, "executor", NULL, -1);
			pipe_fd[STD_IN] = -1;
			pipe_fd[STD_OUT] = -1;
		}
		fd[STD_OUT] = pipe_fd[PIPE_IN];
	}
	else
		fd[STD_OUT] = STD_OUT;
}

void	msh_executor_fork_set_pipe2(t_node *pipe_nd, int *pipe_fd, int *fd)
{
	if (fd[STD_IN] > 2)
		close(fd[STD_IN]);
	if (pipe_nd->right)
	{
		fd[STD_IN] = pipe_fd[PIPE_OUT];
		close(fd[STD_OUT]);
	}
}

int	msh_executor_wait_child(int *pids)
{
	int	i;
	int	statloc;
	int	exit_status;

	i = 0;
	exit_status = 1;
	if (pids[i] == 0)
		return (exit_status);
	while (pids[i])
	{
		if (pids[i] == -1)
			exit_status = -1;
		waitpid(pids[i], &statloc, 0);
		i++;
	}
	if (exit_status == -1)
		return (1);
	exit_status = msh_exit_status(statloc);
	return (exit_status);
}

int	msh_exit_status(int statloc)
{
	if ((statloc & 255) == 0)
		return ((statloc >> 8) & 255);
	return ((statloc & 127) + 128);
}
