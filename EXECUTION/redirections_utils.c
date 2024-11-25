/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anamella <anamella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 22:45:50 by anamella          #+#    #+#             */
/*   Updated: 2024/11/24 21:05:31 by anamella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
else if (redirections->mode == REDIRIN)
	{
		if (redirections->amibiguous_redir == 1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(redirections->file, 2);
			ft_putstr_fd("ambiguous redirect\n", 2);
			return (-1);
		}
		fd = open(redirections->file, O_RDONLY);
	}
*/

int	redirections_type(t_redir *redirections, t_mini *mini)
{
	int	fd;

	(void)mini;
	fd = -1;
	if (redirections->mode == APPEND)
		fd = open(redirections->file, O_CREAT | O_RDWR | O_APPEND, 0644);
	else if (redirections->mode == REDIROUT)
		fd = open(redirections->file, O_CREAT | O_RDWR | O_TRUNC, 0644);
	else if (redirections->mode == REDIRIN)
		fd = open(redirections->file, O_RDONLY);
	else if (redirections->mode == HEREDOC)
		fd = redirections->fd;
	return (fd);
}
/*
if (fd == -1)
{
	if (redirections->amibiguous_redir == 0)
		perror(redirections->file);
	return (1);
}
*/

int	check_redirection(t_tree *root, t_mini *mini)
{
	int		fd;
	t_redir	*redirections;

	redirections = root->data.redirections;
	if (redirections)
	{
		while (redirections)
		{
			fd = redirections_type(redirections, mini);
			if (fd == -1)
			{
				perror(redirections->file);
				return (1);
			}
			if (redirections->mode == APPEND || redirections->mode == REDIROUT)
				dup2(fd, 1);
			else if (redirections->mode == REDIRIN
				|| redirections->mode == HEREDOC)
				dup2(fd, 0);
			close(fd);
			redirections = redirections->next;
		}
	}
	return (0);
}

void	read_heredoc(t_tree *root, t_mini *mini)
{
	t_redir	*red;

	if (root == NULL)
		return ;
	red = root->data.redirections;
	while (red)
	{
		if (red->mode == HEREDOC)
			red->fd = heredoc(red->file, mini);
		red = red->next;
	}
	read_heredoc(root->left, mini);
	read_heredoc(root->right, mini);
}

void	reset_fd(int fd_in, int fd_out)
{
	dup2(fd_in, 0);
	dup2(fd_out, 1);
}
