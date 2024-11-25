/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anamella <anamella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 22:45:19 by anamella          #+#    #+#             */
/*   Updated: 2024/11/24 20:53:13 by anamella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_env	*start_env(void)
{
	t_env	*env;
	char	*pwd;

	pwd = ft_strjoin("PWD=", getcwd(NULL, 0));
	env = malloc(sizeof(t_env));
	if (!env)
		return (NULL);
	add_env(&env, new_env(pwd, (int *)0));
	return (env);
}

t_env	*create_env(char **env)
{
	int		i;
	t_env	*env_l;

	i = 0;
	env_l = NULL;
	if (!env)
		return (start_env());
	while (env[i])
	{
		add_env(&env_l, new_env(env[i], (int *)0));
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
