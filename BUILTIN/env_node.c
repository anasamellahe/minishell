/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_node.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anamella <anamella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 22:44:47 by anamella          #+#    #+#             */
/*   Updated: 2024/11/23 23:21:23 by anamella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	add_env(t_env **env, t_env *new_env)
{
	t_env	*tmp;

	tmp = *env;
	if (!new_env)
		return (-1);
	else if (*env == NULL)
		*env = new_env;
	else
	{
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = new_env;
	}
	return (0);
}

t_env	*new_env(char *s, int *status)
{
	t_env	*env;

	env = malloc(sizeof(t_env));
	if (env == NULL)
	{
		ft_putstr_fd("Error: malloc failed\n", 2);
		return (NULL);
	}
	env->var = get_var(s, status);
	env->val = get_val(s, status);
	env->next = NULL;
	if (env->var == NULL)
	{
		if (env->var)
			free(env->var);
		if (env->val)
			free(env->val);
		free(env);
		return (NULL);
	}
	return (env);
}

void	del_env(t_env **env, char *var)
{
	t_env	*pre;
	t_env	*tmp;

	tmp = *env;
	pre = NULL;
	while (tmp)
	{
		if (!ft_strncmp(tmp->var, var, ft_strlen(var)))
		{
			if (pre == NULL)
				*env = tmp->next;
			else
				pre->next = tmp->next;
			if (tmp->val)
				free(tmp->val);
			if (tmp->var)
				free(tmp->var);
			if (tmp)
				free(tmp);
			return ;
		}
		pre = tmp;
		tmp = tmp->next;
	}
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		if (env->val)
			free(env->val);
		if (env->var)
			free(env->var);
		tmp = env;
		env = env->next;
		if (tmp)
			free(tmp);
	}
}
