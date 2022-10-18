/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_tree_print.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiwolee <jiwolee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 15:38:33 by jiwolee           #+#    #+#             */
/*   Updated: 2022/10/18 16:18:18 by jiwolee          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include	"msh_tree.h"

int	msh_tree_print_node(t_node *node)
{
	if (node == NULL)
		return (-1);
	printf("node type: %d, str1 %s, str2", \
			node->type, node->str1);
	for (int i = 0; node->str2 && node->str2[i]; i++)
		printf(" %s", node->str2[i]);
	printf("\n");
	return (1);
}

int	msh_tree_print(t_node *node)
{
	if (node)
		msh_tree_print_node(node);
	else
		return (-1);
	msh_tree_print(node->left);
	msh_tree_print(node->right);
	return (1);
}
