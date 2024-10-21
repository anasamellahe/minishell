

#include "../includes/minishell.h"

char    *extend_string(t_expand *params)
{
    char *str;

    str = malloc(params->res_size + 2);
    if (!str)
        return NULL;
    if (params->res)
        ft_strlcpy(str, params->res, params->res_size + 2);
    str[params->res_idx++] = params->str[params->i];
    str[params->res_idx] = '\0';
    params->res_size++;
    free(params->res);
    return (str);
}

char    *append_value(t_expand *params, char *value)
{
    size_t value_len;
    char *str;

    if (!value)
        value = "";
    value_len = ft_strlen(value);
    str = malloc(params->res_size + value_len + 1);
    if (!str)
        return NULL;
    ft_strlcpy(str, params->res, params->res_size + 1);
    ft_strlcat(str, value, params->res_size + value_len + 2);
    params->res_idx += value_len;
    params->res_size += value_len;
    free(params->res);
    return (str);
}
