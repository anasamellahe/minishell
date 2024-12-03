/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anamella <anamella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 18:45:52 by anamella          #+#    #+#             */
/*   Updated: 2024/12/03 20:10:21 by anamella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	write_to_heredoc(char *line, int fd)
{
	ft_putstr_fd(line, fd);
	ft_putchar_fd('\n', fd);
	free(line);
}

int	heredoc(const char *delimiter, t_mini *mini)
{
	int		fd[2];
	char	*line;
	int		pid;

	create_pipe(fd, mini);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		while (1)
		{
			line = readline("> ");
			if (!line || strcmp(line, delimiter) == 0)
				free_heredoc(line, mini);
			line = expand_in_heredoc(line);
			write_to_heredoc(line, fd[1]);
		}
	}
	else if (pid > 0)
		mini->exit = get_exit_status(pid);
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
		return (execute_parenthesis(root, mini));
	return (0);
}
