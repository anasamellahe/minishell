/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anamella <anamella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 22:45:26 by anamella          #+#    #+#             */
/*   Updated: 2024/12/03 18:52:58 by anamella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*path_join(char *path, char *cmd)
{
	char	*tmp;
	char	*tmp2;

	tmp = ft_strjoin(path, "/");
	tmp2 = ft_strjoin(tmp, cmd);
	free(tmp);
	return (tmp2);
}

int	check_path(char *cmd, int *status)
{
	struct stat	st;

	stat(cmd, &st);
	if (ft_strchr(cmd, '/'))
	{
		if (S_ISDIR(st.st_mode))
			return (errno = 21, perror(cmd), *status = 126, 1);
		if (access(cmd, F_OK) == -1)
		{
			perror(cmd);
			*status = 127;
			return (1);
		}
		if (access(cmd, F_OK | X_OK) != 0)
		{
			perror(cmd);
			*status = 126;
			return (1);
		}
		*status = 0;
		return (1);
	}
	return (0);
}

int	join_command(t_tree *root, t_env **env, int *status)
{
	char	**path;
	char	*cmd_path;
	int		i;

	if (root->data.cmd == NULL)
		return (*status = 1, 1);
	if (check_path(root->data.cmd, status))
		return (*status);
	path = ft_split(get_env("PATH", *env), ':');
	i = 0;
	while (path && path[i])
	{
		cmd_path = path_join(path[i], root->data.cmd);
		if (access(cmd_path, F_OK) == 0)
		{
			root->data.cmd = cmd_path;
			return (free_(path), 0);
		}
		free(cmd_path);
		i++;
	}
	*status = 127;
	return (free_(path), cmd_error(root->data.cmd), 1);
}

int	check_builtin(t_tree *root, t_mini *mini, int *exit)
{
	int		i;
	int		(*builtin_f[6])(char **, t_mini *);
	char	**builtin;

	builtin = (char *[]){"echo", "cd", "pwd", "export", "unset", "exit", NULL};
	builtin_f[0] = &echo_f;
	builtin_f[1] = &cd_f;
	builtin_f[2] = &pwd_f;
	builtin_f[3] = &export_f;
	builtin_f[4] = &unset_f;
	builtin_f[5] = &exit_f;
	i = 0;
	if (ft_strcmp(root->data.cmd, "env") == 0)
		return (*exit = env_f(root->data.args, mini->env, 0), 1);
	while (builtin[i])
	{
		if (ft_strcmp(root->data.cmd, builtin[i]) == 0)
			return (*exit = builtin_f[i](&root->data.args[1], mini), 1);
		i++;
	}
	*exit = 0;
	return (0);
}

int	get_exit_status(int pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WEXITSTATUS(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (status);
}
