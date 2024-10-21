#include "includes/minishell.h"
#define MAX_BUFF  100
int main (int ac, char **av, char **ev)
{
    t_env *env;
    t_list  *list;
    (void)ac;
    (void)av;
    env = create_env(ev);
    char buff[MAX_BUFF];
    getcwd(buff, MAX_BUFF);
    printf("%s\n", buff);
    while(1)
    {
        char *input = readline("minishell$ ");
        list = lexer(input);
        parser(list);
        if (!strncmp(list->data.cmd.cmd, "env", ft_strlen("env")))
            env_f(env);
        else if (!strncmp(list->data.cmd.cmd, "export", ft_strlen("export")))
            export_f(&env, list->data.cmd.args);
        else if (!strncmp(list->data.cmd.cmd, "unset", ft_strlen("unset")))
            unset_f(&env, list->data.cmd.args);
        else if (!strncmp(list->data.cmd.cmd, "exit", ft_strlen("exit")))
            break;
        else
            printf("Command not found\n");
    }
    
  
}


//execution the code 