/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anamella <anamella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 02:23:35 by anamella          #+#    #+#             */
/*   Updated: 2024/11/30 22:49:41 by anamella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int		g_global_exit = 0;

void	sig_hand(int sig)
{
	(void)sig;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_global_exit = 130;
}

int	convert_and_execute(t_mini *mini)
{
	parser(mini->list, mini->env);
	mini->root = convert_to_ast(mini->list);
	flush_list(mini->list);
	mini->list = NULL;
	mini->char_env = convert_env(mini->env);
	if (read_heredoc(mini->root, mini) == 1)
		return (g_global_exit = mini->exit, 1);
	g_global_exit = execute_ast(mini->root, mini);
	// free_and_reset(mini);
	return (0);
}

void	get_input(t_mini *mini)
{
	char	*input;

	while (1)
	{
		signal(SIGINT, sig_hand);
		mini->exit = 0;
		input = readline(BLUE "mminishell$ " RESET);
		if (!input)
			break ;
		add_history(input);
		mini->list = lexer(input);
		free(input);
		if (check_syntax_errors(mini->list))
		{
			free_list(mini->list);
			clear_history();
			mini->list = NULL;
			continue ;
		}
		convert_and_execute(mini);
		free_and_reset(mini);
	}
	clear_history();
}

int	main(int ac, char **av, char **ev)
{
	t_mini	*mini;
	int		exit_statu;

	(void)ac;
	(void)av;
	signal(SIGQUIT, SIG_IGN);
	mini = create_mini(ev);
	get_input(mini);
	exit_statu = mini->exit;
	free_mini(mini);
	exit(exit_statu);
	return (0);
}
