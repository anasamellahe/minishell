/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_f.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anamella <anamella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 22:44:40 by anamella          #+#    #+#             */
/*   Updated: 2024/11/30 19:15:10 by anamella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	env_f(char **args, t_env *env, int flag)
{
	char	*val;

	if (args && args[1])
		return (1);
	while (env)
	{
		if (flag == 1)
		{
			val = env->val;
			if (val)
				printf("declare -x %s=\"%s\"\n", env->var, env->val);
			else
				printf("declare -x %s\n", env->var);
		}
		else if (env->var && env->val)
			printf("%s=%s\n", env->var, env->val);
		env = env->next;
	}
	return (0);
}
