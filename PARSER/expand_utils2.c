/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 23:03:53 by aderraj           #+#    #+#             */
/*   Updated: 2024/11/23 17:40:10 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
 *	vars[0]: i;
 *	vars[1]: start;
 *	vars[2]: current array_size;
 *	vars[3]: current array_idx;
*/

char	**add_word(char **words, char *s, int *vars)
{
	char	*var_name;
	char	*to_add;
	char	*value;

	var_name = NULL;
	value = NULL;
	to_add = ft_substr(s, vars[1], vars[0] - vars[1]);
	words = extend_array(words, to_add, vars[3]++, &vars[2]);
	if (s[vars[0] + 1])
		var_name = get_varname(&s[vars[0] + 1], &vars[0]);
	if (var_name)
		value = getenv(var_name);
	if (value)
		words = extend_array(words, value, vars[3]++, &vars[2]);
	vars[1] = vars[0];
	free(var_name);
	free(to_add);
	return (words);
}

char	*merge_array(char **arr, int arr_size)
{
	char	*joined;
	int		i;
	int		j;
	int		size;

	if (!arr)
		return (NULL);
	i = -1;
	size = 0;
	while (++i < arr_size)
	{
		if (arr[i])
			size += ft_strlen(arr[i]);
	}
	joined = malloc(sizeof(char) * (size + 1));
	i = -1;
	j = 0;
	while (joined && ++i < arr_size)
	{
		size = 0;
		while (arr[i][size])
			joined[j++] = arr[i][size++];
	}
	joined[j] = 0;
	return (free_array(arr), joined);
}

char	*expand_in_heredoc(char *s)
{
	char	**words;
	char	*tmp;
	int		vars[4];

	ft_bzero(vars, sizeof(int) * 4);
	tmp = NULL;
	words = NULL;
	while (s && s[vars[0]])
	{
		if (s[vars[0]] == '$')
			words = add_word(words, s, vars);
		if (s[vars[0]])
			vars[0]++;
	}
	if (vars[0] - vars[1])
	{
		tmp = ft_substr(s, vars[1], vars[0] - vars[1]);
		words = extend_array(words, tmp, vars[3], &vars[2]);
	}
	free(tmp);
	free(s);
	return (merge_array(words, vars[2]));
}
