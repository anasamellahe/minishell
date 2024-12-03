/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 07:15:04 by marvin            #+#    #+#             */
/*   Updated: 2024/12/03 05:59:26 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_operator(char c)
{
	return (c == '|' || c == '>' || c == '<');
}

int	ft_isspace(char c)
{
	return (c == ' ' || (c <= 13 && c >= 9));
}

int	get_array_len(char **array)
{
	int	i;

	i = 0;
	while (array[i])
		i++;
	return (i);
}

void	append_redirection(t_redir **redirection, t_redir *new)
{
	t_redir	*tmp;

	tmp = *redirection;
	if (tmp)
	{
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	else
		*redirection = new;
}

bool	innormal_var(t_expand *params, t_list *node)
{
	t_list	*tmp;

	tmp = node;
	if (!node)
		return (true);
	if (node->prev)
	{
		while (tmp && !params->quotes_flags[1])
		{
			if (tmp->type == OR || tmp->type == AND)
				node->data.delayed_expand_flag++;
			tmp = tmp->prev;
		}
	}
	if (node->data.delayed_expand_flag)
	{
		if (params->quotes_flags[0] && node->data.delayed_expand_flag < 2)
			params->res = append_value(params, "\"");
		return ((void)(params->res = extend_string(params), params->i++), true);
	}
	if (params->quotes_flags[1] || !params->str[params->i + 1])
		return ((void)(params->res = extend_string(params), params->i++), true);
	if (params->str[params->i + 1] == '?')
		return (expand_exit_status(params, g_global_exit), true);
	return (false);
}
