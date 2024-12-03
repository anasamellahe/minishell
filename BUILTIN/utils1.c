/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anamella <anamella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 22:45:19 by anamella          #+#    #+#             */
/*   Updated: 2024/12/03 19:10:26 by anamella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_env	*create_env(char **env)
{
	int		i;
	t_env	*env_l;
	char	*var;
	char	*val;

	i = 0;
	env_l = NULL;
	while (env && env[i])
	{
		var = get_var(env[i]);
		val = get_val(env[i]);
		add_env(&env_l, new_env(var, val));
		i++;
	}
	return (env_l);
}

char	*get_env(char *name, t_env *env)
{
	if (!name)
		return (NULL);
	while (env)
	{
		if (!ft_strcmp(name, env->var))
			return (env->val);
		env = env->next;
	}
	return (NULL);
}

int	cd_free(void *p, int f)
{
	if (p && f)
	{
		free(p);
		p = NULL;
	}
	return (0);
}
