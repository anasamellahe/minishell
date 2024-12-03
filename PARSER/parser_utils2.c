/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 05:01:32 by aderraj           #+#    #+#             */
/*   Updated: 2024/12/03 22:52:10 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	check_wildcard(t_expand *params, t_list *node)
{
	if (params->quotes_flags[0] || params->quotes_flags[1])
	{
		params->res = extend_string(params);
		params->i++;
		return (false);
	}
	else if (node->s && *node->s && !node->s[1] && node->prev
		&& (node->prev->type == REDIRIN || node->prev->type == REDIROUT
			|| node->prev->type == APPEND || node->prev->type == HEREDOC))
	{
		params->res = extend_string(params);
		params->i++;
		node->ambiguous_flag = true;
		return (false);
	}
	return (true);
}

void	sort_fnames(t_list *start, t_list *end)
{
	t_list	*ptr1;
	t_list	*lptr;
	int		swapped;

	if (!start)
		return ;
	lptr = NULL;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		ptr1 = start;
		while (ptr1->next != lptr && ptr1->next != end)
		{
			if (ft_strncmp(ptr1->s, ptr1->next->s, ft_strlen(ptr1->s) + 1) > 0)
			{
				swap_strings(ptr1, ptr1->next);
				swapped = 1;
			}
			ptr1 = ptr1->next;
		}
		lptr = ptr1;
	}
}

void	remove_heredeoc_quotes(t_list *list)
{
	t_expand	params;

	if (list->type == HEREDOC && list->next)
	{
		if (!list->next->s)
			return ;
		ft_bzero(&params, sizeof(t_expand));
		params.str = list->next->s;
		while (params.str && params.str[params.i])
		{
			if (params.str[params.i] == '\'' || params.str[params.i] == '"')
				set_quotes_flags(&params);
			else
			{
				params.res = extend_string(&params);
				params.i++;
			}
		}
		free(list->next->s);
		list->next->s = params.res;
	}
}

void	expand_delayed_exit_status(char **args, int *idx)
{
	t_expand	params;
	char		*tmp;

	ft_bzero(&params, sizeof(t_expand));
	expand_exit_status(&params, g_global_exit);
	tmp = params.res;
	params.str = params.res;
	params.res = NULL;
	insert_value(&params, args, *idx, 2);
	free(tmp);
}

bool	special_expansion(t_expand *params, t_list *node, char *value)
{
	if (!value && !params->quotes_flags[0] && node->prev
		&& (node->prev->type == REDIRIN || node->prev->type == REDIROUT
			|| node->prev->type == APPEND || node->prev->type == HEREDOC))
	{
		node->ambiguous_flag = true;
		params->i = 0;
		params->res = extend_string(params);
		return (true);
	}
	return (false);
}
