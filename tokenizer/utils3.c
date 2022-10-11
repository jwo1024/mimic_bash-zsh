/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeyjeon <@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 22:40:20 by jaeyjeon          #+#    #+#             */
/*   Updated: 2022/10/10 22:42:07 by jaeyjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "tokenizer.h"

char	*del_dequot(char *s)
{
	char	*new_str;
	t_index	*idx;

	idx = make_idx();
	new_str = malloc(sizeof(char) * (ft_strlen(s) + 1));
	while (s[idx->i] != '\0')
	{
		if (s[idx->i] == '\"')
			do_del_dequot(new_str, s, '\"', idx);
		else if (s[idx->i] == '\'')
			do_del_dequot(new_str, s, '\'', idx);
		else
			new_str[idx->j++] = s[idx->i++];
	}
	new_str[idx->j] = '\0';
	free (s);
	free (idx);
	return (new_str);
}

void	do_del_dequot(char *new_str, char *s, char c, t_index *idx)
{
	if (check_dequot(&s[idx->i]))
	{
		idx->i++;
		while (s[idx->i] != c)
			new_str[idx->j++] = s[idx->i++];
		idx->i++;
	}
	else
		new_str[idx->j++] = s[idx->i++];
}