#include "../includes/minishell.h"
static int ft_free(void *p)
{
	if (p)
		free(p);
	p = NULL;
	return (0);
}

static int get_arg(char **path)
{
	int i;

	i = 0;
	while (path && path[i])
		i++;
	return (i);
}
static void set_wd(char *wd, t_env **env)
{
	char *tmp;

	tmp = ft_strjoin("OLDPWD=", wd);
	if (set_env("OLDPWD", wd, env))
		add_env(env, new_env(tmp));
	ft_free(tmp);
	ft_free(wd);
	wd = getcwd(NULL, 0);
	tmp = ft_strjoin("PWD=", wd);
	if (set_env("PWD", wd, env))
		add_env(env, new_env(tmp));
	ft_free(tmp);
	ft_free(wd);
}
static int change_dir(char *path, t_env **env)
{
	char *old_pwd;

	old_pwd = getcwd(NULL, 0);
	if (!path || !ft_strncmp(path, "~", ft_strlen(path)))
	{
		path = get_env("HOME", *env);
		if (!path)
			return (ft_putstr_fd("cd: HOME not set\n", 2), ft_free(old_pwd),1);
	}
	else if (path && !ft_strncmp(path, "-", ft_strlen(path)))
	{
			path = get_env("OLDPWD", *env);
			if (!path)
				return (ft_putstr_fd("cd: OLDPWD not set\n", 2), ft_free(old_pwd), 1);
	}
	if (chdir(path))
		return (ft_putstr_fd("cd: ", 2), perror(path), ft_free(old_pwd), 1);
	set_wd(old_pwd, env);
	return (0);
}
int cd_f(char **path, t_env **env)
{
	int arg;

	arg = get_arg(path);
	if (arg > 1)
		return (ft_putstr_fd("cd: too many arguments\n", 2), 1);
	if (arg == 0)
		return(change_dir(NULL, env));
	if (arg == 1)
		return(change_dir(path[0], env));
	return (0);
}