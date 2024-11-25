/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anamella <anamella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 02:23:35 by anamella          #+#    #+#             */
/*   Updated: 2024/11/25 01:03:08 by anamella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	sig_hand(int sig)
{
	(void)sig;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	get_input(t_mini *mini)
{
	char	*input;

	while (1)
	{
		input = readline(BLUE "mminishell$ " RESET);
		add_history(input);
		if (!input)
			break ;
		mini->list = lexer(input);
		free(input);
		parser(mini->list);
		mini->root = convert_to_ast(mini->list);
		read_heredoc(mini->root, mini);
		mini->char_env = convert_env(mini->env);
		mini->exit = execute_ast(mini->root, mini);
		free_and_reset(mini);
	}
	clear_history();
}

int	main(int ac, char **av, char **ev)
{
	t_mini	*mini;
	int		exit_statu;

	printf("parent == %d\n", getpid());
	(void)ac;
	(void)av;
	signal(SIGINT, sig_hand);
	mini = create_mini(ev);
	get_input(mini);
	exit_statu = mini->exit;
	free_mini(mini);
	exit(exit_statu);
	return (0);
}
