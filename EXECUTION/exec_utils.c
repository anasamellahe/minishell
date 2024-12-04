/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anamella <anamella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 18:46:44 by anamella          #+#    #+#             */
/*   Updated: 2024/12/04 20:12:46 by anamella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	execute_command(t_tree *root, t_mini *mini)
{
	int		status;
	pid_t	pid;

	signal(SIGINT, SIG_IGN);
	if (check_redirection(root, mini) == 1)
		return (EXIT_FAILURE);
	check_delayed_expansion(root, mini->env);
	if (check_builtin(root, mini, &status) == 1)
		return (status);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		if (join_command(root, &mini->env, &status) && status)
			free_exit(mini, status * (status != 1));
		if (execve(root->data.cmd, root->data.args, mini->char_env) == -1)
			error_msg(root->data.cmd, mini, 127);
	}
	else if (pid > 0)
		return (get_exit_status(pid));
	else
		error_msg("fork failed", mini, EXIT_FAILURE);
	return (0);
}

int	execute_and(t_tree *root, t_mini *mini)
{
	int	status_left;

	status_left = execute_ast(root->left, mini);
	reset_fd(mini->infd, mini->outfd);
	g_global_exit = status_left;
	if (status_left == 0)
		return (execute_ast(root->right, mini));
	else
		return (status_left);
}

int	execute_or(t_tree *root, t_mini *mini)
{
	int	status_left;

	status_left = execute_ast(root->left, mini);
	reset_fd(mini->infd, mini->outfd);
	g_global_exit = status_left;
	if (status_left != 0)
		return (execute_ast(root->right, mini));
	else
		return (status_left);
}

int	execute_parenthesis(t_tree *root, t_mini *mini)
{
	int	pid;
	int	status;

	status = 0;
	signal(SIGINT, SIG_IGN);
	check_redirection(root, mini);
	pid = fork();
	if (pid == 0)
	{
		status = execute_ast(root->sub_tree, mini);
		exit(status);
	}
	else if (pid > 0)
		return (get_exit_status(pid));
	else
		error_msg("fork failed", mini, EXIT_FAILURE);
	return (status);
}

int	execute_pipe(t_tree *node, t_mini *mini)
{
	int		pipefd[2];
	pid_t	pid_left;
	pid_t	pid_right;

	signal(SIGINT, SIG_IGN);
	create_pipe(pipefd, mini);
	pid_left = fork();
	if (pid_left == 0)
		child_process1(node, pipefd, mini);
	else if (pid_left > 0)
	{
		pid_right = fork();
		if (pid_right == 0)
			child_process2(node, pipefd, mini);
		else if (pid_right > 0)
			return (end_of_pipe(pid_left, pid_right, pipefd));
		else
			error_msg("fork failed", mini, EXIT_FAILURE);
	}
	else
		error_msg("fork failed", mini, EXIT_FAILURE);
	return (0);
}
