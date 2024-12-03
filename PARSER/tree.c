/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 07:54:21 by marvin            #+#    #+#             */
/*   Updated: 2024/12/04 00:31:07 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
 * stats array[]:
 * 0 : root of the tree
 * 1 : current command
 * 2 : last operator
 * 3 : last pipe
 * 4 : tmp to store new_nodes
 */

// #define ROOT stats[0]
// #define CURRENT_CMD stats[1]
// #define LAST_OP stats[2]
// #define LAST_PIPE stats[3]
// #define TMP stats[4]

t_tree	*new_tree_node(t_token type, t_cmd data)
{
	t_tree	*node;

	node = (t_tree *)malloc(sizeof(t_tree));
	if (!node)
		return (NULL);
	ft_bzero(node, sizeof(t_tree));
	node->type = type;
	node->data = data;
	return (node);
}

void	insert_pipe(t_cmd data, t_tree *stats[])
{
	int	flag;

	flag = 0;
	stats[4] = new_tree_node(PIPE, data);
	if (!stats[0])
		stats[4]->left = stats[1];
	else if (stats[2])
	{
		stats[2]->right = stats[4];
		stats[4]->left = stats[1];
		stats[2] = NULL;
		flag = 1;
	}
	else
	{
		if (stats[3])
			stats[3]->right = stats[1];
		else
			stats[0]->right = stats[1];
		stats[4]->left = stats[0];
	}
	if (!flag)
		stats[0] = stats[4];
	stats[3] = stats[4];
	stats[1] = NULL;
}

void	insert_logical_op(t_list *node, t_tree *stats[])
{
	stats[4] = new_tree_node(node->type, node->data);
	if (!stats[0])
		set_position(stats);
	else if (stats[3])
	{
		stats[3]->right = stats[1];
		stats[4]->left = stats[0];
		stats[3] = NULL;
	}
	else if (stats[2])
	{
		stats[2]->right = stats[1];
		stats[4]->left = stats[0];
	}
	else
	{
		stats[0]->right = stats[1];
		stats[4]->left = stats[0];
	}
	stats[0] = stats[4];
	stats[2] = stats[4];
	stats[1] = NULL;
}

t_tree	*set_root_node(t_tree *stats[])
{
	if (stats[1] && stats[2])
		stats[2]->right = stats[1];
	else if (stats[1] && stats[0])
	{
		if (stats[3])
			stats[3]->right = stats[1];
		else
			stats[0]->right = stats[1];
	}
	else if (stats[1] && !stats[0])
		stats[0] = stats[1];
	return (stats[0]);
}

t_tree	*convert_to_ast(t_list *list)
{
	t_tree	*stats[5];

	ft_bzero(stats, sizeof(stats));
	while (list && list->s)
	{
		if (list->type == CMD)
			stats[1] = new_tree_node(CMD, list->data);
		else if (list->type == PIPE)
			insert_pipe(list->data, stats);
		else if (list->type == AND || list->type == OR)
			insert_logical_op(list, stats);
		else if (list->type == PARENTHESIS)
		{
			stats[4] = new_tree_node(PARENTHESIS, list->data);
			stats[4]->sub_tree = convert_to_ast(list->sub_list);
			stats[1] = stats[4];
		}
		list = list->next;
	}
	return (set_root_node(stats));
}
