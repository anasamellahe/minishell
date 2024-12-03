/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_delayed.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 01:02:21 by aderraj           #+#    #+#             */
/*   Updated: 2024/12/03 05:56:59 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	insert_value(t_expand *params, char **cmd_args, int idx, int len)
{
	int		i;
	char	*value;
	bool	found;

	if (!cmd_args[idx])
		return ;
	i = 0;
	found = 0;
	value = params->str;
	params->str = cmd_args[idx];
	while (cmd_args[idx][i])
	{
		if (cmd_args[idx][i] == '$' && !found)
		{
			params->res = append_value(params, value);
			i += len - 1;
			params->i += len - 1;
			found = true;
		}
		else
			params->res = extend_string(params);
		params->i++;
		i++;
	}
	(void)(free(cmd_args[idx]), cmd_args[idx] = params->res);
}

char	**insert_subarray(char **array, char **subarray, int *idx)
{
	char	**new_array;
	int		len;
	int		sub_len;
	int		i;

	if (!subarray || !*subarray)
		return (array);
	len = get_array_len(array);
	sub_len = get_array_len(subarray);
	new_array = ft_calloc((len + sub_len + 1), sizeof(char *));
	if (!new_array)
		return (perror("Failed to allocate memory"), NULL);
	i = -1;
	while (++i <= *idx)
		new_array[i] = ft_strdup(array[i]);
	i = -1;
	(*idx)++;
	while (++i < sub_len)
		new_array[*idx + i] = ft_strdup(subarray[i]);
	i = *idx;
	while (i++ < len)
		new_array[i] = ft_strdup(array[i]);
	return (free_array(array), (void)(*idx += sub_len), new_array);
}

char	**split_value(t_expand *params, char **cmd_args, char *value, int *idx)
{
	char	**sub;

	sub = ft_split(value, ' ');
	params->str = sub[0];
	insert_value(params, cmd_args, *idx, params->name_len);
	cmd_args = insert_subarray(cmd_args, &sub[1], idx);
	free_array(sub);
	sub = NULL;
	return (cmd_args);
}

char	**get_var_value(char **cmd_args, int *idx, int j, t_env *env)
{
	t_expand	params;
	char		*var_name;
	char		*value;

	ft_bzero(&params, sizeof(t_expand));
	var_name = get_varname(&cmd_args[*idx][j + 1], &params.name_len);
	if (!var_name || !*var_name)
		return (free(var_name), (*idx)++, cmd_args);
	value = get_env(var_name, env);
	params.str = value;
	if (j > 0 && cmd_args[*idx][j - 1] == '"')
	{
		params.str = str_cut(cmd_args[*idx], j - 1);
		free(cmd_args[*idx]);
		cmd_args[*idx] = params.str;
		params.str = value;
		insert_value(&params, cmd_args, *idx, params.name_len);
	}
	else if (value && params.name_len == (int)ft_strlen(cmd_args[*idx]))
		cmd_args = split_value(&params, cmd_args, value, idx);
	else
		insert_value(&params, cmd_args, *idx, params.name_len);
	return (free(var_name), cmd_args);
}

char	**expand_delayed_var(char **args, t_env *env)
{
	int			i;
	char		*dollar;

	if (!args)
		return (NULL);
	i = 0;
	while (args && args[i])
	{
		dollar = ft_strchr(args[i], '$');
		if (dollar && dollar[1] == '?')
			expand_delayed_exit_status(args, &i);
		else if ((dollar - args[i]) > 0 && dollar[-1] == '"'
			&& !ft_isalnum(dollar[1]) && dollar[1] != '_')
		{
			dollar = str_cut(args[i], (dollar - args[i]) - 1);
			free(args[i]);
			args[i] = dollar;
		}
		else if (dollar)
			args = get_var_value(args, &i, dollar - args[i], env);
		else
			i++;
	}
	return (args);
}
