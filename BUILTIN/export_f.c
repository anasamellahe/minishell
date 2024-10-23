#include "../includes/minishell.h"

void export_f(t_env **env_l, char **env)
{
	int i;

	i = 0;
	if (!env || !*env)
	{
		env_f(*env_l, 1);
		return;
	}
	while (env && env[i])
	{
		if (get_env(get_var(env[i]), *env_l))
			set_env(get_var(env[i]), get_val(env[i]), env_l);
		else
			add_env(env_l, new_env(env[i]));
		i++;
	}
}