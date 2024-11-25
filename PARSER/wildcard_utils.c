/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 12:22:30 by marvin            #+#    #+#             */
/*   Updated: 2024/11/23 17:14:15 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_segment(char *s, int *idx, int len)
{
	char	*str;

	str = malloc((len + 1) * sizeof(char));
	if (!str)
		return (NULL);
	ft_strncpy(str, s, len);
	str[len] = '\0';
	(*idx)++;
	return (str);
}

void	set_segments(t_wildcard *rules)
{
	while (rules->ptr && *rules->ptr)
	{
		if (*rules->ptr == '/')
		{
			if (rules->ptr > rules->start)
			{
				rules->len = rules->ptr - rules->start;
				rules->segments[rules->idx] = get_segment(rules->start,
						&rules->idx, rules->len);
			}
			rules->segments[rules->idx] = get_segment("/", &rules->idx, 1);
			rules->ptr++;
			rules->start = rules->ptr;
		}
		else
			rules->ptr++;
	}
	if (rules->ptr > rules->start)
	{
		rules->len = rules->ptr - rules->start;
		rules->segments[rules->idx] = get_segment(rules->start, &rules->idx,
				rules->len);
		rules->idx--;
	}
}

char	*get_pattern(t_expand *params)
{
	int		i;
	int		len;
	char	*s;

	if (params->str[0] == '*' && !params->str[1])
		return (ft_strdup("*"));
	i = params->i;
	s = NULL;
	while (i >= 0 && !ft_isspace(params->str[i]))
		i--;
	len = params->i;
	while (params->str[len] && !ft_isspace(params->str[len]))
		len++;
	if (i > 1 || len > 1)
		s = ft_substr(params->str, i + 1, len);
	return (s);
}

void	add_match(t_wildcard *rules, char *s)
{
	char	*str;

	str = construct_filename(rules, s);
	if (!rules->match_found && str)
	{
		free(rules->params->res);
		rules->params->res = ft_strdup(str);
		rules->params->res_idx += ft_strlen(str);
		rules->params->res_size = ft_strlen(str);
		rules->match_found = true;
		while (rules->params->str[rules->params->i]
			&& !ft_isspace(rules->params->str[rules->params->i]))
			rules->params->i++;
	}
	else if (str)
		rules->node = insert_node(rules->node, new_node(ft_strdup(str), WORD));
	free(str);
}

void	final_touches(t_wildcard *rules)
{
	int	i;

	free(rules->pattern);
	i = 0;
	while (i < rules->num_segments)
		free(rules->segments[i++]);
	free(rules->segments);
	if (!rules->match_found)
	{
		rules->params->res = extend_string(rules->params);
		rules->params->i++;
	}
	else
		rules->params->to_sort = true;
}
