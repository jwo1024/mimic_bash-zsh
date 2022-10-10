/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeyjeon <@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 14:00:14 by jaeyjeon          #+#    #+#             */
/*   Updated: 2022/10/10 20:27:51 by jaeyjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// 오류처리는 안함.... 다음에 알아보자
int	do_pwd(int *fd)
{
	char	*str;

	str = getcwd(NULL, 0);
	ft_putstr_fd(str, fd[STD_OUT]);
	ft_putstr_fd("\n", fd[STD_OUT]);
	free(str);
	return (0);
}
