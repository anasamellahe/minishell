/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_f.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anamella <anamella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 22:45:02 by anamella          #+#    #+#             */
/*   Updated: 2024/11/25 00:43:46 by anamella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	pwd_f(char **args, t_mini *mini)
{
	char	*buff;
	t_env	**env;

	(void)args;
	env = &mini->env;
	buff = getcwd(NULL, 0);
	if (!buff)
	{
		if (get_env("PWD", *env))
		{
			printf("%s\n", get_env("PWD", *env));
			return (0);
		}
		else
			perror("getcwd");
		return (1);
	}
	printf("%s\n", buff);
	free(buff);
	return (0);
}
