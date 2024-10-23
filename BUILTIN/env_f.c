#include "../includes/minishell.h"

void env_f(t_env *env, int flag)
{
	while(env)
	{
		if (flag == 1)
			printf("declare -x %s=\"%s\"\n", env->var, env->val);
		else
			printf("%s=%s\n", env->var, env->val);
		env = env->next;
	}
}