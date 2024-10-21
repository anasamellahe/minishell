
#include "../includes/minishell.h"

int get_args_count(t_list *list)
{
    int i;
    t_list *tmp;

    i = 0;
    tmp = list;
    while (tmp && tmp->type == WORD)
    {
        i++;
        tmp = tmp->next;
    }
    return (i);
}

t_list    *get_args(t_list *list, u_token_data *data)
{
    int i;
    t_list *tmp;

    i = 0;
    while (list && list->type == WORD)
    {
        list->s = expand_rm_quotes(list->s);
        tmp = list;
        data->cmd.args[i] = ft_strdup(list->s);
        list = list->next;
        free(tmp->s);
        free(tmp);
        i++;
    }
    data->cmd.args[i] = NULL;
    return (list);
}

void    merge_nodes(t_list *list, t_redir *redirs)
{
    int size;

    list->s = expand_rm_quotes(list->s);
    size = get_args_count(list);
    list->data.cmd.args = malloc(sizeof(char *) * (size));
    list->data.cmd.redirections = redirs;
    if (!list->data.cmd.args)
        return ;
    list->data.cmd.cmd = ft_strdup(list->s);
    if (!list->data.cmd.cmd)
    {
        free(list->data.cmd.args);
        return ;   
    }
    list->type = CMD;
    list->next = get_args(list->next, &list->data);
}

t_list *add_redir_node(t_redir **redirections, t_list *list)
{
    t_redir *new;
    t_list *tmp;

    new = malloc(sizeof(t_redir));
    if (!new)
        return (NULL);
    new->mode = list->type;
    tmp = NULL;
    new->next = NULL;
    if (list->next && list->next->type == WORD)
    {
        tmp = list->next->next;
        new->file = ft_strdup(list->next->s);
        free(list->next->s);
        free(list->next);
    }
    while ((*redirections) && (*redirections)->next)
        *redirections = (*redirections)->next;
    if (*redirections)
        (*redirections)->next = new;
    else
        *redirections = new;
    return (tmp);
}

t_list    *get_redirections(t_list *list, t_list *current, t_redir **redirect)
{
    t_list *replace;

    replace = current;
    while (list && list->type != PIPE && list->type != PARENTHESIS
        && list->type != AND && list->type != OR)
    {
        if (list && (list->type == REDIRIN || list->type == REDIROUT
            || list->type == APPEND || list->type == HEREDOC))
        {
            if (list && list->next == current)
            {
                replace = add_redir_node(redirect, list);
                list->next = replace;
            }
            else
                list->next = add_redir_node(redirect, list);
        }
        list = list->next;
    }
    return (replace);
}

void    parser(t_list *list)
{
    t_list *tmp;
    t_redir *redirections;
    t_list *start;

    tmp = list;
    redirections = NULL;
    start = tmp;
    while (tmp)
    {
        if (tmp && tmp->type == WORD)
        {
            tmp = get_redirections(start, tmp, &redirections);
            merge_nodes(tmp, redirections);
            redirections = NULL;
        }
        else if (tmp && tmp->type == PARENTHESIS)
        {
            tmp->sub_list = lexer(&tmp->s[1]);
            parser(tmp->sub_list);
        }
        else if (tmp->type == PIPE || tmp->type == AND || tmp->type == OR)
            start = tmp->next;
        if (tmp)
            tmp = tmp->next;
    }
}

void    print_list(t_list *list)
{
    for (t_list *tmp = list; tmp; tmp = tmp->next)
    {
        printf("node -> {type = [%d], s = [%s]}\n", tmp->type, tmp->s);
        if (tmp->data.cmd.cmd)
            printf("       data -> cmd = [%s]\n", tmp->data.cmd.cmd);
        for (int i = 0; tmp->data.cmd.args && tmp->data.cmd.args[i]; i++)
            printf("       data -> cmd.args = [%s]\n", tmp->data.cmd.args[i]);
        for (t_redir *tmp2 = tmp->data.cmd.redirections; tmp2; tmp2 = tmp2->next)
        {
            printf("       data -> cmd.redirections = {mode = [%d], file = [%s]\n",\
            tmp2->mode, tmp2->file);
        }
        if (tmp->sub_list)
        {
            printf(GREEN"---SUB_list\n"RESET);
            print_list(tmp->sub_list);
            printf(GREEN"---END OF SUB_list\n"RESET);
        }
    }
}