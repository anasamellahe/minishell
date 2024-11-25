/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anamella <anamella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 22:45:42 by anamella          #+#    #+#             */
/*   Updated: 2024/11/24 00:47:01 by anamella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	heredoc(const char *delimiter, t_mini *mini)
{
	int		fd[2];
	char	*line;

	create_pipe(fd, mini);
	while (1)
	{
		line = readline("> ");
		if (!line || strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(fd[1], line, strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
	return (close(fd[1]), fd[0]);
}

void	child_process1(t_tree *root, int *pipefd, t_mini *mini)
{
	int	status;

	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	status = execute_ast(root->left, mini);
	free_exit(mini, status);
}

void	child_process2(t_tree *root, int *pipefd, t_mini *mini)
{
	int	status;

	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	status = execute_ast(root->right, mini);
	free_exit(mini, status);
}

int	execute_ast(t_tree *root, t_mini *mini)
{
	if (root == NULL)
		return (0);
	if (root->type == CMD)
		return (execute_command(root, mini));
	else if (root->type == AND)
		return (execute_and(root, mini));
	else if (root->type == PIPE)
		return (execute_pipe(root, mini));
	else if (root->type == OR)
		return (execute_or(root, mini));
	if (root->type == PARENTHESIS)
		execute_parenthesis(root, mini);
	return (0);
}
