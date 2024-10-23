

#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../libft/libft.h"

# define EXIT_STATUS 0
# define RED "\x1b[31m"
# define GREEN "\x1b[32m"
# define BLUE "\x1b[34m"
# define YELLOW "\x1b[33m"
# define MAGENTA "\x1b[35m"
# define CYAN "\x1b[36m"
# define RESET "\x1b[0m"
# define BOLD "\x1b[1m"
# define ITALIC "\x1b[3m"
# define UNDERLINE "\x1b[4m"


typedef struct s_env
{
	char			*var;
	char			*val;
	struct s_env	*next;
}	t_env;

typedef enum s_token
{
	WORD,
	CMD,
	REDIRIN,
	REDIROUT,
	APPEND,
	HEREDOC,
	PIPE,
	PARENTHESIS,
	AND,
	OR
} e_token;

typedef struct s_redir
{
	e_token mode;
	char *file;
	struct s_redir *next;
} t_redir;

typedef struct s_cmd
{
	char *cmd;
	char **args;
	t_redir *redirections;
} t_cmd;

typedef union
{
	t_cmd cmd;
	struct s_tree *sub_tree;
} u_token_data;

typedef struct s_list
{
	char *s;
	e_token	type;
	u_token_data data;
	t_env *env;
	struct s_list *sub_list;
	struct s_list *prev;
	struct s_list *next;
} t_list;

typedef struct s_expand
{
	char *str;
	int i;
	char *res;
	int res_idx;
	int res_size;
}   t_expand;

typedef struct s_wildcard
{
	DIR *dir;
	struct dirent *entry;
	size_t  name_len;
	int recursive_flag;
	bool    flag;
	char    **fragments;
	char    *pattern;
	char    *current_dir;
}   t_wildcard;

typedef struct s_tree
{
	e_token type;
	u_token_data data;
	struct s_tree *parent;
	struct s_tree *left;
	struct s_tree *right;
} t_tree;

//lexer functions
t_list	*new_node(char *s, int type);
t_list	*lexer(char *s);
void	parse_quotes(char *s, int *i);
void	parse_parenthesis(t_list **list, char *s, int *i);
void	parse_words(t_list **list, char *s, int *i);
void	parse_operators(t_list **list, char *s, int *i);
void	add_node(t_list **node, t_list *new_node);
int		is_operator(char c);
int		ft_isspace(char c);
//end of lexer functions

//parser functions
t_list	*get_redirections(t_list *list, t_list *current, t_redir **redirect);
t_tree	*create_ast_node(t_tree *parent, e_token type, u_token_data data);
t_tree	*convert_to_tree(t_list *list);
t_list 	*get_args(t_list *list, u_token_data *data);
t_list	*add_redir_node(t_redir **redirections, t_list *list);
char	*extend_string(t_expand *params);
char	*append_value(t_expand *params, char *value);
char	*get_varname(char *s, int *j);
void	expand_exit_status(t_expand *params);
void	expand_var(t_expand *params, int flag);
void	set_quotes_flags(t_expand *params, int flags[]);
char	*expand_rm_quotes(char *s);
int		get_args_count(t_list *list);
int		empty_space(char *s, int idx);
void    merge_nodes(t_list *list, t_redir *redirs);
void	parser(t_list *list);
void	print_list(t_list *list);
void	construct_path(t_wildcard *specs, char *s);
char	*get_pattern(char *str, int idx);
void	add_first_filename(t_expand *params, char *match, int match_len);
void	add_match(t_expand *params, t_wildcard *specs);
bool	match_wildcards(char *filename, char **fragments);
void	match_patterns(t_expand *params, t_wildcard *specs);
void	recursive_match(t_expand *params, t_wildcard *specs);
void	fetsh_files(t_expand *params, t_wildcard *specs);
void	expand_wildcards(t_expand *params, int quotes, int dquotes);
//end of parser functions

// env and builtin function 
t_env	*create_env(char **env);
t_env	*new_env(char *s);
void	add_env(t_env **env, t_env *new_env);
void	del_env(t_env **env, char *var);
void	free_env(t_env *env);
char	*get_val(char *s);
char	*get_var(char *s);
void	export_f(t_env **env_l, char **env);
void	unset_f(t_env **env_l, char **env);
void	env_f(t_env *env, int flag);
int		set_env(char *var, char *val, t_env **env);
char	*get_env(char *name, t_env *env);
void	echo_f(char **args);
int		cd_f(char **path, t_env **env);
int		pwd_f(void);
//end of env and builtin funcion

#endif