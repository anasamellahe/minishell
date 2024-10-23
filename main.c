#include "includes/minishell.h"
#define MAX_BUFF  100
int main (int ac, char **av, char **ev)
{
    t_env *env;
    t_list  *list;
    (void)ac;
    (void)av;
    env = create_env(ev);
    while(1)
    {
        char *cwd = ft_strjoin (get_env("PWD", env), "$ ");
        char *input = readline(cwd);
        list = lexer(input);
        parser(list);
        if(!list)
            continue;
        if (!strcmp(list->data.cmd.cmd, "env"))
            env_f(env, 0);
        else if (!strcmp(list->data.cmd.cmd, "export"))
            export_f(&env, list->data.cmd.args);
        else if (!strcmp(list->data.cmd.cmd, "unset"))
            unset_f(&env, list->data.cmd.args);
        else if (!strcmp(list->data.cmd.cmd, "echo"))
            echo_f(list->data.cmd.args);
        else if (!strcmp(list->data.cmd.cmd, "exit"))
            break;
        else if (!strcmp(list->data.cmd.cmd, "cd"))
            cd_f(list->data.cmd.args, &env);
        else if (!strcmp(list->data.cmd.cmd, "pwd"))
            pwd_f();
        else
            print_list(list);
        free(input);
        free(cwd);
    }
    
  
}


//execution the code 