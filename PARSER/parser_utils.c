

#include "../includes/minishell.h"


t_tree *create_ast_node(t_tree *parent, e_token type, u_token_data data)
{
    t_tree *new_node;

    new_node = malloc(sizeof(t_tree));
    if (!new_node)
        return NULL; // Handle memory allocation failure

    ft_bzero(new_node, sizeof(t_tree));
    new_node->type = type;
    new_node->data = data;
    new_node->parent = parent;
    return new_node;
}

t_tree  *convert_to_tree(t_list *list)
{
    t_list *tmp;
    t_tree *root;

    tmp = list;
    root = NULL;
    while (tmp)
    {
        if (tmp->type == AND || tmp->type == OR)
            root = create_ast_node(NULL, tmp->type, tmp->data);
        else if (tmp->type == PARENTHESIS)
        {
            tmp->data.sub_tree = convert_to_tree(tmp->sub_list);
            create_ast_node(root,  PARENTHESIS, tmp->data);
        }
        tmp = tmp->next;
    }
    return (root);
}

