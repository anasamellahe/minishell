#include "../includes/minishell.h"

t_env *create_env(char **env)
{
	int		i;
	t_env	*env_l;

	i = 0;
	env_l = NULL;
	if (!env)
		return (NULL);
	while (env[i])
	{
		add_env(&env_l, new_env(env[i]));
		i++;
	}
	return (env_l);
}
void export_f(t_env **env_l, char **env)
{
	int i;

	i = 0;
	while (env && env[i])
	{
		add_env(env_l, new_env(env[i]));
		i++;
	}
}

void unset_f(t_env **env_l, char **env)
{
	int	i;

	i = 0;
	while (env && env[i])
	{
		del_env(env_l, env[i]);
		i++;
	}
}
void env_f(t_env *env)
{
	while(env)
	{
		printf("%s=%s\n", env->var, env->val);
		env = env->next;
	}
}

char *get_env(char *name, t_env *env)
{
	while (env)
	{
		if (!ft_strncmp(name, env->var, ft_strlen(name)))
			return (env->val);
		env = env->next;
	}
	return (NULL);
}



