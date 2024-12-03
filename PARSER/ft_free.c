/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 08:59:22 by marvin            #+#    #+#             */
/*   Updated: 2024/11/27 22:23:32 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_data(t_cmd *data)
{
	t_redir	*tmp;
	t_redir	*tmp2;
	int		i;

	i = 0;
	if (data->cmd)
	{
		free (data->cmd);
		data->cmd = NULL;
	}
	while (data->args && data->args[i])
	{
		free(data->args[i]);
		i++;
	}
	free(data->args);
	tmp = data->redirections;
	while (tmp)
	{
		tmp2 = tmp;
		tmp = tmp->next;
		free(tmp2->file);
		free(tmp2);
	}
}

void	flush_list(t_list *list)
{
	t_list	*tmp;

	while (list)
	{
		tmp = list;
		list = list->next;
		ft_bzero(&tmp->data, sizeof(t_cmd));
		flush_list(tmp->sub_list);
		free(tmp->s);
		free(tmp);
	}
}
void	free_list(t_list *list)
{
	t_list	*tmp;

	while (list)
	{
		tmp = list;
		list = list->next;
		free_data(&tmp->data);
		free_list(tmp->sub_list);
		free(tmp->s);
		free(tmp);
	}
}

void	free_tree(t_tree *tree)
{
	if (!tree)
		return ;
	free_data(&tree->data);
	free_tree(tree->sub_tree);
	free_tree(tree->left);
	free_tree(tree->right);
	free(tree);
}

void	free_array(char **arr)
{
	int	i;

	i = 0;
	while (arr && arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}
