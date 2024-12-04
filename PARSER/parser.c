/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 20:05:20 by aderraj           #+#    #+#             */
/*   Updated: 2024/12/04 01:57:05 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_list	*get_args(t_list *list, t_cmd *data, int size, t_env *env)
{
	int		i;
	t_list	*tmp;

	i = 1;
	while (list && list->type != PIPE && list->type != AND && list->type != OR
		&& list->type != PARENTHESIS)
	{
		if (list->type == WORD)
		{
			expand_rm_quotes(list, list->s, env);
			if (list->s && data->args)
				data->args = extend_array(data->args, list->s, i++, &size);
		}
		data->ambigous_flag = list->data.ambigous_flag;
		data->delayed_expand_flag = list->data.delayed_expand_flag;
		tmp = list;
		list = list->next;
		if (list)
			list->prev = tmp->prev;
		free(tmp->s);
		free(tmp);
	}
	return (list);
}

void	merge_words(t_list *list, t_redir *redirs, t_env *env)
{
	int		size;
	char	*tmp;

	tmp = ft_strdup(list->s);
	if (list->type == WORD)
		expand_rm_quotes(list, list->s, env);
	if (tmp && !ft_strchr(tmp, '"') && !ft_strchr(tmp, '"')
		&& ft_strchr(tmp, '$') && !*list->s && list->next)
		list = list->next;
	size = get_args_count(list);
	if (size)
	{
		list->data.args = ft_calloc(size + 1, sizeof(char *));
		if (!list->data.args)
			return ((void)(free(tmp)));
	}
	if (list->s && size)
	{
		list->data.cmd = ft_strdup(list->s);
		list->data.args[0] = ft_strdup(list->data.cmd);
		if (!list->data.cmd)
			return ((void)(free(tmp)));
	}
	(void)(free(tmp), list->data.redirections = redirs, list->type = CMD);
	list->next = get_args(list->next, &list->data, size, env);
}

t_list	*add_redir_node(t_redir **redirections, t_list *list)
{
	t_redir	*new;
	t_list	*tmp;

	tmp = NULL;
	if (!list->next || list->next->type != WORD)
		return (list->next);
	new = malloc(sizeof(t_redir));
	if (!new)
		return (NULL);
	new->ambiguous = list->next->ambiguous_flag;
	new->mode = list->type;
	new->next = NULL;
	tmp = list->next->next;
	new->file = ft_strdup(list->next->s);
	free(list->next->s);
	free(list->next);
	list->next = tmp;
	append_redirection(redirections, new);
	return (tmp);
}

t_list	*get_redirections(t_list *list, t_list *current, t_redir **redirect,
		t_env *env)
{
	t_list	*replace;

	replace = current;
	while (list && list->type != PIPE && list->type != PARENTHESIS
		&& list->type != AND && list->type != OR)
	{
		if (list && (list->type == REDIRIN || list->type == REDIROUT
				|| list->type == APPEND || list->type == HEREDOC))
		{
			remove_heredeoc_quotes(list);
			if (list->type != HEREDOC && list->next)
				expand_rm_quotes(list->next, list->next->s, env);
			if (list->next == current)
			{
				replace = add_redir_node(redirect, list);
				if (!replace)
					return (list);
				list->next = replace;
			}
			else
				list->next = add_redir_node(redirect, list);
		}
		list = list->next;
	}
	return (replace);
}

void	parser(t_list *list, t_env *env)
{
	t_list	*tmp[3];
	t_redir	*redirections;

	tmp[0] = list;
	redirections = NULL;
	tmp[1] = list;
	tmp[2] = NULL;
	while (tmp[0])
	{
		arrange_nodes(tmp, &redirections, env);
		if (tmp[0])
			tmp[0] = tmp[0]->next;
	}
}
