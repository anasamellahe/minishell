/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anamella <anamella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 22:45:14 by anamella          #+#    #+#             */
/*   Updated: 2024/11/28 17:28:19 by anamella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*ft_strndup(char *str, int size)
{
	char	*s;
	int		i;
	int		len;

	i = 0;
	len = ft_strlen(str);
	if (len == 0)
		return (NULL);
	size = (size * (len >= size)) + (len * (len < size));
	s = malloc((size + 1) * sizeof(char));
	if (!s)
		return (ft_putstr_fd("Error: malloc failed\n", 2), NULL);
	while (str && str[i] && i < size)
	{
		s[i] = str[i];
		i++;
	}
	s[i] = 0;
	return (s);
}

char	*get_val(char *s)
{
	int	i;

	i = 0;
	while (s && s[i])
	{
		if (s[i] == '=')
			return (ft_strndup(&s[i + 1], ft_strlen(&s[i + 1])));
		i++;
	}
	return (NULL);
}

char	*get_var(char *s)
{
	int	i;

	i = 0;
	while (s && s[i])
	{
		if (s[i] == '=' && i > 0)
			return (ft_strndup(s, i));
		i++;
	}
	if (s)
		return (ft_strndup(s, i));
	return (NULL);
}

int	set_env(char *var, char *val, t_env **env)
{
	t_env	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (!ft_strcmp(var, tmp->var))
		{
			if (tmp->var)
				free(tmp->var);
			tmp->var = var;
			if (tmp->val)
				free(tmp->val);
			tmp->val = val;
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}
