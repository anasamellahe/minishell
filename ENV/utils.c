#include "../includes/minishell.h"

static char *ft_strndup(char *str, int size)
{
    char    *s;
    int     i;
    int     len;
    
    i = 0;
    len = ft_strlen(str);
    size = (size * (len >= size)) + (len * (len < size));
    s = malloc ((size + 1) * sizeof(char));
    if (!s)
        return(ft_putstr_fd("Error: malloc failed\n", 2), NULL);
    while (str && str[i] && i < size)
    {
        s[i] = str[i];
        i++;
    }
    s[i] = 0;
    return (s);
}

char *get_val(char *s)
{
	int i;

	i = 0;
	while (s && s[i])
	{
		if (s[i] == '=')
			return (ft_strndup(&s[i + 1], ft_strlen(&s[i + 1])));
		i++;
	}
	return (NULL);
}

char *get_var(char *s)
{
	int	i;

	i = 0;
	while (s && s[i])
	{
		if (s[i] == '=')
			return (ft_strndup(s, i));
		i++;
	}
	return (NULL);
}