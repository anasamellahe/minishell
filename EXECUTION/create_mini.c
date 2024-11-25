/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_mini.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anamella <anamella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 18:25:29 by anamella          #+#    #+#             */
/*   Updated: 2024/11/25 02:02:23 by anamella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_mini	*create_mini(char **env)
{
	t_mini	*mini;

	mini = malloc(sizeof(t_mini));
	if (!mini)
		error_msg("malloc fail", NULL, EXIT_FAILURE);
	mini->env = create_env(env);
	mini->char_env = NULL;
	mini->list = NULL;
	mini->root = NULL;
	mini->exit = 0;
	mini->infd = dup(0);
	if (mini->infd == -1)
	{
		free_mini(mini);
		error_msg("dup failed", NULL, EXIT_FAILURE);
	}
	mini->outfd = dup(1);
	if (mini->outfd == -1)
	{
		free_mini(mini);
		error_msg("dup failed", NULL, EXIT_FAILURE);
	}
	return (mini);
}

void	free_mini(t_mini *mini)
{
	free_env(mini->env);
	if (mini->list)
	{
		free_list(mini->list);
		mini->list = NULL;
	}
	if (mini->root)
	{
		free_tree(mini->root);
		mini->root = NULL;
	}
	if (mini->char_env)
	{
		free_char_env(mini->char_env);
		mini->char_env = NULL;
	}
	free(mini);
	mini = NULL;
}

void	free_and_reset(t_mini *mini)
{
	reset_fd(mini->infd, mini->outfd);
	free_list(mini->list);
	free_tree(mini->root);
	free_char_env(mini->char_env);
	mini->list = NULL;
	mini->root = NULL;
	mini->char_env = NULL;
}
