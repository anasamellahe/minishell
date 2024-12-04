/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_syntax.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 23:29:53 by aderraj           #+#    #+#             */
/*   Updated: 2024/12/04 01:43:35 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	report_error(char *token, int flag)
{
	if (flag == -2)
		ft_putstr_fd(" syntax error : unclosed parenthesis\n", 2);
	else if (flag == -1)
		ft_putstr_fd(" syntax error : unclosed quotes\n", 2);
	else if (flag == 0)
	{
		ft_putstr_fd(" syntax error near unexpected token `", 2);
		ft_putstr_fd(token, 2);
	}
	else if (flag == 1)
	{
		ft_putstr_fd(" ambigious redirection `", 2);
		ft_putstr_fd(token, 2);
	}
	else if (flag == 2)
		ft_putstr_fd(" maximum here-document count exceeded\n", 2);
	if (flag >= 0 && flag != 2)
		ft_putstr_fd("'\n", 2);
}

bool	is_empty_string(char *str)
{
	int	i;

	if (!str)
		return (false);
	i = 0;
	while (str[i] && ft_isspace(str[i]))
		i++;
	if (!str[i])
		return (true);
	return (false);
}

bool	check_parenthesis(t_list *node)
{
	t_list	*tmp;

	if (node->type == PARENTHESIS)
	{
		if (is_empty_string(node->s))
			return (report_error(")", 0), true);
		else if ((node->prev && node->prev->type != OR
				&& node->prev->type != AND && node->prev->type != PIPE)
			|| (node->next && (node->next->type == WORD
					|| node->next->type == PARENTHESIS)))
		{
			if (node->next && node->next->type == PARENTHESIS)
				return (report_error("(", 0), true);
			else if (node->next == WORD)
				return (report_error(node->next->s, 0), true);
			tmp = lexer(node->s);
			return (report_error(tmp->s, 0), free_list(tmp), true);
		}
	}
	return (false);
}

bool	check_redirections(t_list *node)
{
	static int	n_heredocs;

	if (node->type == REDIRIN || node->type == REDIROUT || node->type == APPEND
		|| node->type == HEREDOC)
	{
		if (node->type == HEREDOC)
			n_heredocs++;
		if (n_heredocs >= 17)
			return (report_error(NULL, 2), true);
		if (!node->next)
			return (report_error("newline", 0), true);
		else if (node->next->type != WORD && node->next->type != PARENTHESIS)
			return (report_error(node->next->s, 0), true);
		else if (node->next->type == PARENTHESIS)
			return (report_error("(", 0), true);
	}
	return (false);
}

bool	check_syntax_errors(t_list *list, char *input)
{
	t_list	*tmp;

	if (quotes_parenthesis_errors(input, 0))
		return (true);
	tmp = list;
	while (tmp)
	{
		if (tmp->s && (!ft_strcmp(tmp->s, "&") || !ft_strcmp(tmp->s, ";")))
			return (report_error(tmp->s, 0), (void)(g_global_exit = 2), true);
		if (check_parenthesis(tmp) || check_redirections(tmp))
			return ((void)(g_global_exit = 2), true);
		else if (tmp->type == PIPE || tmp->type == OR || tmp->type == AND)
		{
			if (!tmp->prev || tmp->prev->type == OR || tmp->prev->type == AND
				|| tmp->prev->type == PIPE || !tmp->next
				|| tmp->next->type == OR || tmp->next->type == AND
				|| tmp->next->type == PIPE)
				return (report_error(tmp->s, 0), (void)(g_global_exit = 2),
					true);
		}
		tmp = tmp->next;
	}
	return (false);
}
