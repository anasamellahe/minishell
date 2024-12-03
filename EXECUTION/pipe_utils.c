/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anamella <anamella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 22:45:47 by anamella          #+#    #+#             */
/*   Updated: 2024/11/26 18:04:14 by anamella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	*create_pipe(int *pipefd, t_mini *mini)
{
	if (pipe(pipefd) == -1)
		error_msg("pipe failed", mini, EXIT_FAILURE);
	return (pipefd);
}

void	close_pipe(int *pipefd)
{
	close(pipefd[0]);
	close(pipefd[1]);
}

int	end_of_pipe(pid_t pid_left, pid_t pid_right, int *pipefd)
{
	int	status;

	close(pipefd[0]);
	close(pipefd[1]);
	status = get_exit_status(pid_left);
	status = get_exit_status(pid_right);
	return (status);
}

int	close_fd(int fd1, int fd2)
{
	if (fd1 != -1)
		close(fd1);
	if (fd2 != -1)
		close(fd2);
	return (0);
}
