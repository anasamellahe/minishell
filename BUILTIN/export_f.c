/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_f.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anamella <anamella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 22:44:57 by anamella          #+#    #+#             */
/*   Updated: 2024/12/03 18:51:08 by anamella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_in_env(char *name, t_env *env)
{
	while (env)
	{
		if (!ft_strcmp(name, env->var))
			return (1);
		env = env->next;
	}
	return (0);
}

void	export_error(char *name)
{
	ft_putstr_fd("export: `", 2);
	ft_putstr_fd(name, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	free(name);
}

int	check_var(char *name)
{
	int	i;

	i = 0;
	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (export_error(name), 1);
	while (name[i])
	{
		if (!ft_isalpha(name[i]) && !ft_isdigit(name[i]) && name[i] != '_')
			return (export_error(name), 1);
		i++;
	}
	return (0);
}

int	check_and_add(char *s, t_env **env_l)
{
	char	*var;
	char	*val;

	var = get_var(s);
	if (check_var(var))
		return (1);
	val = get_val(s);
	if (is_in_env(var, *env_l))
		set_env(var, val, env_l);
	else
		add_env(env_l, new_env(var, val));
	return (0);
}

int	export_f(char **env, t_mini *mini)
{
	int		i;
	int		exit_status;
	t_env	**env_l;

	i = 0;
	env_l = &mini->env;
	if (!env || !*env)
		return (env_f(NULL, *env_l, 1), 0);
	exit_status = 0;
	while (env && env[i])
	{
		if (check_and_add(env[i], env_l))
			exit_status = 1;
		i++;
	}
	return (exit_status);
}
