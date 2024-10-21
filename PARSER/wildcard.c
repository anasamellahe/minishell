

#include "../includes/minishell.h"

bool    match_wildcards(char *filename, char **fragments)
{
    int i;
    char *pos;

    i = 0;
    pos = filename;
    if (!fragments[i])
        return false;
    while (fragments[i])
    {
        pos = ft_strnstr(pos, fragments[i], ft_strlen(pos));
        if (!pos) 
            return false;
        pos += ft_strlen(fragments[i]);
        i++;
    }
    return true;
}

void    match_patterns(t_expand *params, t_wildcard *specs)
{
    specs->fragments = ft_split(specs->pattern, '*');
    if (specs->fragments)
        specs->fragments[0] += ft_strlen(specs->current_dir) - 1;
    while ((specs->entry = readdir(specs->dir)))
    {
        if (specs->entry->d_name[0] == '.')
            continue;
        specs->name_len = ft_strlen(specs->entry->d_name);
        if (!specs->fragments)
            add_match(params, specs);
        else if (specs->entry->d_type == DT_DIR\
        && specs->fragments && !ft_strncmp(specs->entry->d_name,\
        specs->fragments[0], specs->name_len))
            recursive_match(params, specs);
        else if (match_wildcards(specs->entry->d_name, specs->fragments))
            add_match(params, specs);
    }
}

void    recursive_match(t_expand *params, t_wildcard *specs)
{
    DIR *subdir;
    t_wildcard sub_specs;
    char subdir_path[254];

    construct_path(specs, subdir_path);
    subdir = opendir(subdir_path);
    if (subdir)
    {
        specs->recursive_flag = 1;
        sub_specs = *specs;
        sub_specs.dir = subdir;
        sub_specs.current_dir = subdir_path;
        match_patterns(params, &sub_specs);
        closedir(subdir);
    }
}

void   fetsh_files(t_expand *params, t_wildcard *specs)
{
    specs->current_dir = ".";
    specs->dir = opendir(".");
    if (!specs->dir)
        return;
    match_patterns(params, specs);
    closedir(specs->dir);
    free(specs->fragments);
}

void expand_wildcards(t_expand *params, int quotes, int dquotes)
{
    t_wildcard  specs;

    ft_memset(&specs, 0, sizeof(t_wildcard));
    if (quotes || dquotes)
    {
        params->res = extend_string(params);
        params->i++;
        return;
    }
    specs.pattern = get_pattern(params->str, params->i);
    fetsh_files(params, &specs);
    if (specs.flag || specs.recursive_flag)
    {
        while(params->str[params->i] && !ft_isspace(params->str[params->i]))
            params->i++;
    }
    else
    {
        params->res = extend_string(params);
        params->i++;
    }
    free(specs.pattern);
    return;
}
