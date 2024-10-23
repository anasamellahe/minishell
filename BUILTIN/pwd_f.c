#include "../includes/minishell.h"

int pwd_f(void)
{
    char buff[100];
    if (!getcwd(buff, 100))
    {
        perror("getcwd");
        return (1);
    }
    printf("%s\n", buff);
    return (0);
}