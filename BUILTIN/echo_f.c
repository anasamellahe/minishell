#include "../includes/minishell.h"

static int check_arg(char *s)
{
	int i;

	i = 0;
	while (s && s[i])
	{
		if ((s[i] != 'n' && i != 0) || (s[i] != '-' && i == 0))
			return (0);
		i++;
	}
	return (1 * (i != 0 && i >= 2));
}
void echo_f(char **args)
{
	int new_line;
	int i;

	i = 0;
	if (args == NULL)
	{
		printf("\n");
		return;
	}
	new_line = check_arg(args[0]); 
	i += new_line;
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (new_line == 0)
		printf("\n");
}