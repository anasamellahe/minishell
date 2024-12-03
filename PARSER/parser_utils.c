/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 22:41:03 by marvin            #+#    #+#             */
/*   Updated: 2024/12/03 05:04:52 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	set_redirections(t_list *node, t_list *last_parenthesis)
{
	t_redir	*new_redir;
	t_list	*to_free;

	if (node->type == REDIRIN || node->type == REDIROUT || node->type == APPEND
		|| node->type == HEREDOC)
	{
		if (last_parenthesis)
		{
			new_redir = malloc(sizeof(t_redir));
			if (!new_redir)
				return ;
			new_redir->mode = node->type;
			new_redir->file = ft_strdup(node->next->s);
			new_redir->next = last_parenthesis->data.redirections;
			last_parenthesis->data.redirections = new_redir;
			to_free = node->next;
			node->next = node->next->next;
			free(to_free->s);
			free(to_free);
		}
	}
}

void	set_lonely_redirections(t_list *list[3], t_redir **redirections,
		t_env *env)
{
	t_list	*dummy_cmd;

	if ((!list[0]->prev || list[0]->prev->type != WORD) && (!list[0]->next
			|| list[0]->next->type != WORD))
	{
		dummy_cmd = new_node(NULL, CMD);
		if (!dummy_cmd)
			return ;
		dummy_cmd->next = list[0];
		if (list[0])
			list[0]->prev = dummy_cmd;
		list[0] = dummy_cmd;
	}
	if (list[0]->next && (!list[0]->next->next
			|| list[0]->next->next->type != WORD))
	{
		list[0] = get_redirections(list[1], list[0], redirections, env);
		merge_words(list[0], *redirections, env);
		*redirections = NULL;
	}
}

void	arrange_nodes(t_list *list[3], t_redir **redirections, t_env *env)
{
	if (list[0] && list[0]->type == WORD)
	{
		list[0] = get_redirections(list[1], list[0], redirections, env);
		merge_words(list[0], *redirections, env);
		*redirections = NULL;
	}
	else if (list[0] && list[0]->type == PARENTHESIS)
	{
		list[0]->sub_list = lexer(list[0]->s);
		parser(list[0]->sub_list, env);
		list[2] = list[0];
	}
	else if (list[0] && (list[0]->type == PIPE || list[0]->type == AND
			|| list[0]->type == OR))
	{
		list[1] = list[0]->next;
		list[2] = NULL;
	}
	else if (list[0] && (!list[0]->prev || list[0]->prev->type != PARENTHESIS)
		&& (list[0]->type == REDIRIN || list[0]->type == REDIROUT
			|| list[0]->type == APPEND || list[0]->type == HEREDOC))
		set_lonely_redirections(list, redirections, env);
	else
		set_redirections(list[0], list[2]);
}

char	*construct_filename(t_wildcard *rules, char *s)
{
	char	*str;
	char	*tmp;

	if (!rules->skip_hidden)
		str = ft_strdup(s + 2);
	else
		str = ft_strtrim(s, "./");
	if (rules->add_slash && str)
	{
		tmp = ft_strjoin(str, "/");
		free(str);
		str = tmp;
	}
	return (str);
}

void	set_position(t_tree *stats[])
{
	if (stats[3])
	{
		stats[3]->right = stats[1];
		stats[4]->left = stats[3];
	}
	else
		stats[4]->left = stats[1];
	stats[0] = stats[4];
}
