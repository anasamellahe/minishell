#include "../includes/minishell.h"

char **get_PATH(t_env *env)
{
	char *path;
	char **paths;

	path = get_env("PATH", env);
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	return (paths);
}
char *valid_cmd(char *cmd, char **env)
{
	char **paths;
	char *tmp;
	char *tmp2;
	int i;

	i = 0;
	paths = get_PATH(env);
	if (!paths)
		return (NULL);
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		tmp2 = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!access(tmp2, F_OK))
		{
			
			free_tab(paths);
			return (tmp2);
		}
		free(tmp2);
		i++;
	}
	free_tab(paths);
	return (NULL);
}
void free_tab(char **tab)
{
	int i;

	i = 0;
	while (tab && tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}