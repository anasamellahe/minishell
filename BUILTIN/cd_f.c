/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_f.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anamella <anamella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 22:44:27 by anamella          #+#    #+#             */
/*   Updated: 2024/11/25 00:40:22 by anamella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	get_arg(char **path)
{
	int	i;

	i = 0;
	while (path && path[i])
		i++;
	return (i);
}

static void	set_wd(char *wd, t_env **env)
{
	char	*tmp;

	tmp = ft_strjoin("OLDPWD=", wd);
	if (set_env(ft_strdup("OLDPWD"), ft_strdup(wd), env))
		add_env(env, new_env(tmp, (int *)0));
	cd_free(tmp, 1);
	cd_free(wd, 1);
	wd = getcwd(NULL, 0);
	tmp = ft_strjoin("PWD=", wd);
	if (set_env(ft_strdup("PWD"), ft_strdup(wd), env))
		add_env(env, new_env(tmp, (int *)0));
	cd_free(tmp, 1);
	cd_free(wd, 1);
}

static char	*join_path(char *path, t_env *env)
{
	char	*home;

	home = get_env("HOME", env);
	if (!home)
		return (ft_putstr_fd("cd: HOME not set\n", 2), NULL);
	if (!path || !ft_strncmp(path, "~", ft_strlen(path)))
		return (ft_strdup(home));
	else if (!ft_strncmp(path, "~/", 2))
		return (ft_strjoin(home, &path[1]));
	else
		return (ft_strdup(path));
}

static int	change_dir(char *path, t_env **env)
{
	char	*old_pwd;
	int		f;

	old_pwd = getcwd(NULL, 0);
	f = 0;
	if (!path || !ft_strncmp(path, "~", 1))
	{
		f = 1;
		path = join_path(path, *env);
		if (!path)
			return (cd_free(old_pwd, 1), 1);
	}
	else if (path && !ft_strncmp(path, "-", ft_strlen(path)))
	{
		path = get_env("OLDPWD", *env);
		if (!path)
			return (ft_putstr_fd("cd: OLDPWD not set\n", 2), cd_free(old_pwd,
					1), cd_free(path, f), 1);
		printf("%s\n", path);
	}
	if (chdir(path))
		return (ft_putstr_fd("cd: ", 2), perror(path), cd_free(old_pwd, 1),
			cd_free(path, f), 1);
	cd_free(path, f);
	return (set_wd(old_pwd, env), 0);
}

int	cd_f(char **path, t_mini *mini)
{
	int		arg;
	t_env	**env;

	env = &mini->env;
	arg = get_arg(path);
	if (arg > 1)
		return (ft_putstr_fd("cd: too many arguments\n", 2), 1);
	if (arg == 0)
		return (change_dir(NULL, env));
	if (arg == 1)
		return (change_dir(path[0], env));
	return (0);
}
