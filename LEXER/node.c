

#include "../includes/minishell.h"

void add_node(t_list **node, t_list *new_node)
{
	t_list *tmp;

	tmp = *node;
	if (*node == NULL)
		*node = new_node;
	else
	{
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = new_node;
		new_node->prev = tmp;
	}
}

t_list *new_node(char *s, int type)
{
	t_list *node;

	node = malloc(sizeof(t_list));
	if (node == NULL)
		return (NULL);
	ft_bzero(node, sizeof(t_list));
	node->type = type;
	node->s = s;
	return (node);
}
