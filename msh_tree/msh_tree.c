#include	"./msh_tree.h"

#include	<stdio.h>
#include	<stdlib.h>

t_tree *msh_tree_create_tree()
{
	t_tree *new_tree;

	new_tree = (t_tree *)malloc(sizeof(t_tree));
	if (new_tree == NULL) // null 가드 해줘 말아
		return (NULL);
	new_tree->top = NULL;
	new_tree->node_count = 0;
	return(new_tree);
}


t_node *msh_tree_create_node(enum e_ttype type, char *str)
{
	t_node *new_node;

	new_node = (t_node *)malloc(sizeof(t_node));
	if (new_node == NULL) // null 가드?
		return (NULL);
	new_node->type = type;
	new_node->str = str;
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->parent = NULL;
	return (new_node);
}

int	msh_tree_add_node_left(t_node *cur_node, t_node *new_node)
{
	if (cur_node->left != NULL)
		return (-1);
	cur_node->left = new_node;
	new_node->parent = cur_node;
	return (1);
}

int	msh_tree_add_node_right(t_node *cur_node, t_node *new_node)
{
	if (cur_node->right != NULL)
		return (-1);
	cur_node->right = new_node;
	new_node->parent = cur_node;
	return (1);
}

int	msh_tree_add_edge_right(t_tree *tree, t_node *new_node)
{
	t_node *cur_node;

	if (tree == NULL || new_node == NULL)
		return (-1);
	else if (tree->top == NULL)
		tree->top = cur_node;
	else
	{
		cur_node = tree->top;
		while (cur_node->right)
			cur_node = cur_node->right;
		msh_tree_add_node_right(cur_node, new_node);
	}
	return (1);
}
