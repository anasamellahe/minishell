/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 07:21:23 by marvin            #+#    #+#             */
/*   Updated: 2024/11/29 02:01:49 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	get_args_count(t_list *list)
{
	int		i;
	t_list	*tmp;

	i = 0;
	tmp = list;
	while (tmp && tmp->type != PIPE && tmp->type != AND && tmp->type != OR
		&& tmp->type != PARENTHESIS)
	{
		if (tmp->s && tmp->type == WORD)
			i++;
		tmp = tmp->next;
	}
	return (i);
}

char	*extend_string(t_expand *params)
{
	char	*str;

	str = malloc(params->res_size + 2);
	if (!str)
		return (NULL);
	if (params->res)
		ft_strlcpy(str, params->res, params->res_size + 2);
	str[params->res_idx++] = params->str[params->i];
	str[params->res_idx] = '\0';
	params->res_size++;
	free(params->res);
	return (str);
}

char	*append_value(t_expand *params, char *value)
{
	size_t	value_len;
	char	*str;

	if (!value)
		value = "";
	value_len = ft_strlen(value);
	str = malloc(params->res_size + value_len + 1);
	if (!str)
		return (NULL);
	ft_strlcpy(str, params->res, params->res_size + 1);
	ft_strlcat(str, value, params->res_size + value_len + 2);
	params->res_idx += value_len;
	params->res_size += value_len;
	free(params->res);
	return (str);
}

char	**extend_array(char **arr, char *new, int i, int *size)
{
	char	**new_arr;

	new_arr = arr;
	if (i >= *size)
	{
		new_arr = malloc(sizeof(char *) * (*size + 2));
		if (!new_arr)
			return (NULL);
		new_arr[*size + 1] = NULL;
		i = 0;
		while (i < *size)
		{
			new_arr[i] = arr[i];
			i++;
		}
		new_arr[i] = ft_strdup(new);
		free(arr);
		*size += 1;
	}
	else
		new_arr[i] = ft_strdup(new);
	return (new_arr);
}

void	split_node(t_list *node)
{
	char	**nodes;
	t_list	*tmp;
	int		i;

	tmp = node->next;
	nodes = ft_split(node->s, ' ');
	if (nodes && *nodes)
	{
		free(node->s);
		node->s = ft_strdup(*nodes);
		i = 1;
		while (nodes[i])
		{
			node->next = new_node(ft_strdup(nodes[i]), WORD);
			node = node->next;
			i++;
		}
		node->next = tmp;
	}
	free_array(nodes);
}
