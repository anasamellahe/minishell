

int is_operator(char c)
{
    return ((c == '|' || c == '>' || c == '<'\
    || c ==  '&' || c == '(' || c == ')'));
}

int ft_isspace(char c)
{
    return (c == ' ' || (c <= 13 && c >= 9));
}
