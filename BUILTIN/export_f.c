/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_f.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anamella <anamella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 22:44:57 by anamella          #+#    #+#             */
/*   Updated: 2024/11/25 00:49:56 by anamella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_var(char *name, t_env *env)
{
	while (env)
	{
		if (!ft_strcmp(name, env->var))
			return (free(name), 1);
		env = env->next;
	}
	if (name)
	{
		free(name);
		name = NULL;
	}
	return (0);
}

// get_env(get_var(env[i], &status), *env_l)
int	export_f(char **env, t_mini *mini)
{
	int		i;
	int		status;
	int		exit;
	t_env	**env_l;

	i = 0;
	status = 0;
	env_l = &mini->env;
	if (!env || !*env)
		return (env_f(*env_l, 1), 0);
	exit = 0;
	while (env && env[i])
	{
		if (check_var(get_var(env[i], &status), *env_l))
			set_env(get_var(env[i], &status), get_val(env[i], &status), env_l);
		else
			add_env(env_l, new_env(env[i], &status));
		i++;
		if (status == 1)
			exit = 1;
		status = 0;
	}
	return (exit);
}
