#include "../includes/minishell.h"

static t_env *start_env(void)
{
	t_env	*env;
	char	*pwd;

	pwd = ft_strjoin("PWD=", getcwd(NULL, 0));
	env = malloc(sizeof(t_env));
	if (!env)
		return (NULL);
	add_env(&env, new_env(pwd));
	return (env);
}
t_env *create_env(char **env)
{
	int		i;
	t_env	*env_l;

	i = 0;
	env_l = NULL;
	if (!env)
		return (start_env());
	while (env[i])
	{
		add_env(&env_l, new_env(env[i]));
		i++;
	}
	return (env_l);
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