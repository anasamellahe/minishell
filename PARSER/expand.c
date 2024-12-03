/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 03:36:24 by marvin            #+#    #+#             */
/*   Updated: 2024/12/03 05:07:43 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_varname(char *s, int *j)
{
	int	i;

	i = 0;
	while (s[i] && (ft_isalnum(s[i]) || s[i] == '_'))
		i++;
	*j += i + 1;
	return (ft_substr(s, 0, i));
}

void	expand_exit_status(t_expand *params, int exit_status)
{
	char	*value;

	value = ft_itoa(exit_status);
	params->res = append_value(params, value);
	if (!params->res)
		return ;
	free(value);
	params->i += 2;
}

void	expand_var(t_expand *params, t_env *env, t_list *node)
{
	char	*var_name;
	char	*value;

	if (innormal_var(params, node) == true)
		return ;
	var_name = get_varname(&params->str[params->i + 1], &params->i);
	if (var_name && !var_name[0])
	{
		params->res = append_value(params, "$");
		free(var_name);
		return ;
	}
	value = get_env(var_name, env);
	if (special_expansion(params, node, value))
		params->i++;
	else if (!params->quotes_flags[0] && value)
		params->to_split = true;
	if (value)
		params->res = append_value(params, value);
	else
		params->res = append_value(params, "");
	free(var_name);
}

void	set_quotes_flags(t_expand *params)
{
	if (params->str[params->i] == '\'' && !params->quotes_flags[0])
	{
		params->quotes_flags[1] = !params->quotes_flags[1];
		if (params->i > 0 && params->str[params->i - 1] == '\'')
			params->res = append_value(params, "");
	}
	else if (params->str[params->i] == '\'')
		params->res = extend_string(params);
	if (params->str[params->i] == '"' && !params->quotes_flags[1])
	{
		params->quotes_flags[0] = !params->quotes_flags[0];
		if (params->i > 0 && params->str[params->i - 1] == '"')
			params->res = append_value(params, "");
	}
	else if (params->str[params->i] == '"')
		params->res = extend_string(params);
	params->i++;
}

void	expand_rm_quotes(t_list *node, char *s, t_env *env)
{
	t_expand	params;

	if (node->expand_flag)
		return ;
	(void)(ft_bzero(&params, sizeof(t_expand)), params.str = s);
	while (s && s[params.i])
	{
		if (s[params.i] != '\'' && s[params.i] != '"' && s[params.i] != '$'
			&& s[params.i] != '*')
		{
			params.res = extend_string(&params);
			params.i++;
		}
		else if (s[params.i] == '\'' || s[params.i] == '"')
			set_quotes_flags(&params);
		else if (s[params.i] == '$')
			expand_var(&params, env, node);
		else if (s[params.i] == '*')
			expand_wildcards(&params, node);
	}
	(void)(free(s), node->s = params.res);
	if (params.to_split)
		split_node(node);
	if (params.to_sort)
		sort_fnames(node, params.idx_node->next);
}
