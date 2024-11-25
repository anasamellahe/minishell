/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anamella <anamella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 22:45:37 by anamella          #+#    #+#             */
/*   Updated: 2024/11/25 01:39:43 by anamella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	execute_command(t_tree *root, t_mini *mini)
{
	int		status;
	int		cmd_exit_status;
	pid_t	pid;

	if (check_redirection(root, mini) == 1)
		return (EXIT_FAILURE);
	if (check_builtin(root, mini, &cmd_exit_status) == 1)
		return (cmd_exit_status);
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
	{
		waitpid(pid, &status, 0);
		return (WEXITSTATUS(status));
	}
	else
		error_msg("fork failed", mini, EXIT_FAILURE);
	return (0);
}

int	execute_and(t_tree *root, t_mini *mini)
{
	int	status_left;

	status_left = execute_ast(root->left, mini);
	if (status_left == 0)
		return (execute_ast(root->right, mini));
	else
		return (status_left);
}

int	execute_or(t_tree *root, t_mini *mini)
{
	int	status_left;

	status_left = execute_ast(root->left, mini);
	if (status_left != 0)
		return (execute_ast(root->right, mini));
	else
		return (status_left);
}

int	execute_parenthesis(t_tree *root, t_mini *mini)
{
	int	status;
	int	pid;

	status = 0;
	pid = fork();
	if (pid == 0)
	{
		status = execute_ast(root->sub_tree, mini);
		exit(status);
	}
	else if (pid > 0)
		waitpid(pid, &status, 0);
	else
		error_msg("fork failed", mini, EXIT_FAILURE);
	return (WEXITSTATUS(status));
}

int	execute_pipe(t_tree *node, t_mini *mini)
{
	int		pipefd[2];
	pid_t	pid_left;
	pid_t	pid_right;

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
