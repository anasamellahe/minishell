/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 04:05:37 by marvin            #+#    #+#             */
/*   Updated: 2024/11/29 02:52:42 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	recursive_match(t_wildcard *rules, char *curr_path, int seg_idx);

void	init_rules(t_wildcard *rules, t_expand *params)
{
	rules->params = params;
	rules->start = rules->pattern;
	rules->ptr = rules->pattern;
	rules->num_segments = 0;
	rules->skip_hidden = 1;
	while (rules->ptr && *rules->ptr)
	{
		if (*rules->ptr == '/')
		{
			if (rules->ptr > rules->start)
				rules->num_segments++;
			rules->num_segments++;
			rules->ptr++;
			rules->start = rules->ptr;
		}
		else
		{
			if (*rules->ptr == '.' && rules->ptr == rules->pattern)
				rules->skip_hidden = 0;
			rules->ptr++;
		}
	}
	if (rules->ptr > rules->start)
		rules->num_segments++;
	rules->segments = malloc(rules->num_segments * sizeof(char *));
}

bool	ft_fnmatch(char *pattern, char *str)
{
	while (*pattern)
	{
		if (*pattern == '*')
		{
			if (!*(++pattern))
				return (false);
			while (*str)
			{
				if (ft_fnmatch(pattern, str) == false)
					return (false);
				str++;
			}
			return (true);
		}
		else
		{
			if (*pattern != *str)
				return (true);
			pattern++;
			str++;
		}
	}
	return (*str != 0);
}

void	match_files(t_wildcard *rules, struct dirent *entry, char *curr_path,
		int seg_idx)
{
	char	*tmp_path;

	tmp_path = ft_strjoin(curr_path, "/");
	free(rules->next_path);
	rules->next_path = ft_strjoin(tmp_path, entry->d_name);
	rules->curr_path = curr_path;
	free(tmp_path);
	tmp_path = ft_strdup(rules->next_path);
	if (rules->pattern[ft_strlen(rules->pattern) - 1] == '/')
		rules->add_slash = 1;
	if (ft_fnmatch(rules->segments[seg_idx], entry->d_name) == 0)
	{
		if (seg_idx == rules->num_segments - 1)
			add_match(rules, rules->next_path);
		else if (ft_strncmp(rules->segments[seg_idx + 1], "/", 2) == 0)
		{
			if (entry->d_type == DT_DIR)
				recursive_match(rules, tmp_path, seg_idx + 2);
		}
		else
			recursive_match(rules, rules->curr_path, seg_idx + 1);
	}
	(void)(free(tmp_path), free(curr_path));
	free(rules->next_path);
	rules->next_path = NULL;
}

// Function to recursively match patterns and traverse directories
void	recursive_match(t_wildcard *rules, char *curr_path, int segm_idx)
{
	struct dirent	*entry;
	DIR				*dir;

	if (segm_idx >= rules->num_segments)
		return ((void)add_match(rules, curr_path));
	dir = opendir(curr_path);
	if (!dir)
		return ;
	entry = readdir(dir);
	while (entry)
	{
		if (rules->skip_hidden && entry->d_name[0] == '.')
		{
			entry = readdir(dir);
			continue ;
		}
		match_files(rules, entry, ft_strdup(curr_path), segm_idx);
		entry = readdir(dir);
	}
	closedir(dir);
}

void	expand_wildcards(t_expand *params, t_list *node)
{
	t_wildcard	rules;
	char		*cwd;

	if (params->quotes_flags[0] || params->quotes_flags[1])
	{
		params->res = extend_string(params);
		params->i++;
		return ;
	}
	else if (node->prev && (node->prev->type == REDIRIN || node->prev->type == REDIROUT
		|| node->prev->type == APPEND || node->prev->type == HEREDOC))
	{
		params->res = extend_string(params);
		params->i++;
		node->ambiguous_flag = true;
		return ;
	}
	ft_bzero(&rules, sizeof(t_wildcard));
	rules.node = node;
	rules.pattern = get_pattern(params);
	init_rules(&rules, params);
	rules.ptr = rules.pattern;
	rules.start = rules.pattern;
	rules.idx = 0;
	set_segments(&rules);
	cwd = ft_strdup(".");
	recursive_match(&rules, cwd, 0);
	rules.params->idx_node = rules.node;
	free(cwd);
	final_touches(&rules);
}
