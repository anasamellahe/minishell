#include "../includes/minishell.h"

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