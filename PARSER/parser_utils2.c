/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anamella <anamella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 05:01:32 by aderraj           #+#    #+#             */
/*   Updated: 2024/12/03 22:00:34 by anamella         ###   ########.fr       */
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
	int		i;
	int		size;
	char	*tmp;

	if (list->type == HEREDOC && list->next)
	{
		if (!list->next->s)
			return ;
		i = 0;
		size = 0;
		if (*list->next->s != '\'' && *list->next->s != '"')
			size++;
		while (list->next->s[i])
			i++;
		size = i;
		if (list->next->s[i - 1] == '\'' || list->next->s[i - 1] == '"')
			size--;
		tmp = list->next->s;
		list->next->s = ft_substr(list->next->s, (*list->next->s == '\''
					|| *list->next->s == '"') + 0, size - 1);
		free(tmp);
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
