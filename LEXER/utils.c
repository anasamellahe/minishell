/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 07:15:04 by marvin            #+#    #+#             */
/*   Updated: 2024/11/29 01:21:35 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_operator(char c)
{
	return (c == '|' || c == '>' || c == '<');
}

int	ft_isspace(char c)
{
	return (c == ' ' || (c <= 13 && c >= 9));
}

void	append_redirection(t_redir **redirection, t_redir *new)
{
	t_redir	*tmp;

	tmp = *redirection;
	if (tmp)
	{
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	else
		*redirection = new;
}

bool	innormal_var(t_expand *params)
{
	if (params->quotes_flags[1] || !params->str[params->i + 1])
	{
		params->res = extend_string(params);
		params->i++;
		return (true);
	}
	if (params->str[params->i + 1] == '?')
	{
		expand_exit_status(params, g_global_exit);
		return (true);
	}
	return (false);
}
