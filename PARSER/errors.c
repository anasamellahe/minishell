/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderraj <aderraj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 05:05:39 by marvin            #+#    #+#             */
/*   Updated: 2024/11/29 02:54:29 by aderraj          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool    check_ambiguous_redirect(t_list *list)
{
    t_redir *tmp;

    while (list)
    {
        tmp = list->data.redirections;
        while (tmp)
        { 
            if (tmp->ambiguous)
            {
                ft_putstr_fd(tmp->file, 2);
                ft_putstr_fd(": ambiguous redirect\n", 2);
                return (true);
            }
            tmp = tmp->next;
        }
        list = list->next;
    }
    return (false);
}
