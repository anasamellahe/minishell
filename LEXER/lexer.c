/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 03:13:26 by marvin            #+#    #+#             */
/*   Updated: 2024/11/29 17:27:08 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	parse_quotes(char *s, int *i)
{
	int		j;
	char	delim;

	if (!s)
		return ;
	j = 1;
	delim = '\'' * (s[0] == '\'') + '"' * (s[0] == '"');
	while (s[j] && s[j] != delim)
		j++;
	if (s[j] != delim)
		j--;
	*i += j;
}

void	parse_parenthesis(t_list **list, char *s, int *i)
{
	int	j;
	int	flag;

	if (!s)
		return ;
	j = 0;
	flag = 0;
	while (s[j])
	{
		if (s[j] == '(')
			flag++;
		else if (s[j] == ')')
			flag--;
		if (!flag)
			break ;
		j++;
	}
	add_node(list, new_node(ft_substr(&s[1], 0, j - 1), PARENTHESIS));
	if (s[j] != ')')
		j--;
	*i += j + 1;
}

void	parse_words(t_list **list, char *s, int *i)
{
	int	j;

	if (!s)
		return ;
	j = 0;
	while (s[j] && !ft_isspace(s[j]) && !is_operator(s[j]))
	{
		if (s[j] == '(' || (s[j] == '&' && s[j + 1] && s[j + 1] == '&'))
			break ;
		if (s[j] == '"' || s[j] == '\'')
			parse_quotes(&s[j], &j);
		j++;
	}
	if (j)
		add_node(list, new_node(ft_substr(s, 0, j), WORD));
	*i += j;
}

void	parse_operators(t_list **list, char *s, int *i)
{
	int	token;

	token = 0;
	if (s[0] == '>')
		token = APPEND * (s[1] && s[1] == '>') + REDIROUT * (!s[1]
				|| s[1] != '>');
	else if (s[0] == '<')
		token = HEREDOC * (s[1] && s[1] == '<') + REDIRIN * (!s[1]
				|| s[1] != '<');
	else if (s[0] == '|')
		token = OR * (s[1] && s[1] == '|') + PIPE * (!s[1] || s[1] != '|');
	else if (s[0] == '&' && s[1] && s[1] == '&')
		token = AND;
	else if (!s[1] || s[1] != '&')
		return ;
	add_node(list, new_node(ft_substr(s, 0, 1 + (token == HEREDOC
					|| token == APPEND || token == OR || token == AND)),
			token));
	*i += (token == APPEND || token == HEREDOC || token == OR || token == AND);
}

t_list	*lexer(char *s)
{
	int		i;
	char	*tmp;
	t_list	*list;

	i = 0;
	list = NULL;
	tmp = ft_substr(s, 0, ft_strlen(s));
	while (tmp && tmp[i])
	{
		if (!ft_isspace(tmp[i]) && tmp[i] != '(')
			parse_words(&list, &tmp[i], &i);
		if (tmp[i] == '(')
			parse_parenthesis(&list, &tmp[i], &i);
		if (tmp[i] && (tmp[i] == '|' || tmp[i] == '>' || tmp[i] == '<'
				|| tmp[i] == '&' || tmp[i] == '*'))
			parse_operators(&list, &tmp[i], &i);
		i += (tmp[i] != '\0');
	}
	return (free(tmp), list);
}
