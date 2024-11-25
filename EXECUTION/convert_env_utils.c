/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_env_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anamella <anamella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 18:09:41 by anamella          #+#    #+#             */
/*   Updated: 2024/11/24 18:10:28 by anamella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**count_env(t_env *env)
{
	int		i;
	char	**ev;

	i = 0;
	ev = NULL;
	while (env)
	{
		if (env->var && env->val)
			i++;
		env = env->next;
	}
	if (i > 0)
	{
		ev = malloc(sizeof(char *) * (i + 1));
		if (!ev)
		{
			perror("malloc");
			return (NULL);
		}
	}
	else
		return (NULL);
	return (ev);
}

void	create_char_env(char **ev, t_env *env)
{
	int		i;
	char	*tmp;

	i = 0;
	tmp = NULL;
	while (env)
	{
		if (env->val && env->var)
		{
			tmp = ft_strjoin(env->var, "=");
			ev[i] = ft_strjoin(tmp, env->val);
			if (tmp)
				free(tmp);
			tmp = NULL;
			i++;
		}
		env = env->next;
	}
	ev[i] = NULL;
}

char	**convert_env(t_env *env)
{
	char	**ev;

	ev = count_env(env);
	if (!ev)
		return (NULL);
	create_char_env(ev, env);
	return (ev);
}

void	free_char_env(char **env)
{
	int	i;

	i = 0;
	while (env && env[i])
	{
		if (env[i])
		{
			free(env[i]);
			env[i] = NULL;
		}
		i++;
	}
	if (env)
	{
		free(env);
		env = NULL;
	}
}
