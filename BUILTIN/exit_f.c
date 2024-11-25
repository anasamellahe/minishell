/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_f.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anamella <anamella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 22:44:52 by anamella          #+#    #+#             */
/*   Updated: 2024/11/24 18:11:10 by anamella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	count_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

/*
	arg[0] >> i;
	arg[1] >> num;
	arg[2] >> space;
	arg[3] >> sign;
*/
int	check_arg(char *s)
{
	int	arg[4];

	ft_bzero(arg, sizeof(int) * 4);
	while (s[arg[0]])
	{
		if (s[arg[0]] == '-' || s[arg[0]] == '+')
		{
			if (arg[1])
				return (0);
			arg[3]++;
		}
		if (!ft_isdigit(s[arg[0]]) && !ft_isspace(s[arg[0]]) && s[arg[0]] != '-'
			&& s[arg[0]] != '+')
			return (1);
		if (ft_isdigit(s[arg[0]]))
			arg[1] = 1;
		if (ft_isspace(s[arg[0]]) && arg[1])
			arg[2] = 1;
		if ((arg[2] && ft_isdigit(s[arg[0]])) || arg[3] > 1)
			return (1);
		arg[0]++;
	}
	return (0);
}

int	get_exit(char *s, t_mini *mini)
{
	if (s == NULL)
		return (mini->exit);
	if (check_arg(s))
	{
		ft_putstr_fd("exit: ", 2);
		ft_putstr_fd(s, 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		exit(2);
	}
	else
		return (ft_atoi(s));
}

int	exit_f(char **args, t_mini *mini)
{
	int	count;
	int	status;

	count = count_args(args);
	printf("%s\n", "exit");
	status = get_exit(args[0], mini);
	if (count > 1)
	{
		printf("minishell: exit: too many arguments\n");
		return (1);
	}
	else
	{
		free_mini(mini);
		exit(status);
	}
}
