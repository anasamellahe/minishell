#include "../includes/minishell.h"

void add_env(t_env **env, t_env *new_env)
{
	t_env *tmp;

	tmp = *env;
	if (*env == NULL)
		*env = new_env;
	else
	{
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = new_env;
	}
}
t_env *new_env(char *s)
{
	t_env *env;

	env = malloc(sizeof(t_env));
	if (env == NULL)
    {
        ft_putstr_fd("Error: malloc failed\n", 2);
		return (NULL);
    }
	env->var = get_var(s);
	env->val = get_val(s);
	env->next = NULL;
    if (env->var == NULL || env->val == NULL)
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

void del_env(t_env **env, char *var)
{
    t_env *pre;
    t_env *tmp;
    
    tmp = *env;
    pre = NULL;
    while (tmp)
    {
        if (!ft_strncmp(tmp->var , var, ft_strlen(var)))
        {
            if (pre == NULL)
                *env = tmp->next;
            else
                pre->next = tmp->next;
			free(tmp->val);
			free(tmp->var);
            free(tmp);
            return;
        }
        pre = tmp;
        tmp = tmp->next;
    }
}
void free_env(t_env *env)
{
    t_env *tmp;
    while (env)
    {
        free(env->val);
        free(env->var);
        tmp = env;
        env = env->next;
        if (tmp)
            free(tmp);
    }
}