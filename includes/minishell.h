/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anamella <anamella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 19:52:14 by aderraj           #+#    #+#             */
/*   Updated: 2024/11/24 18:22:53 by anamella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <dirent.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <signal.h>

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

typedef struct s_mini
{
	struct s_env	*env;
	struct s_tree	*root;
	struct s_list	*list;
	char			**char_env;
	int				exit;
	int				infd;
	int				outfd;
}					t_mini;

typedef struct s_env
{
	char			*var;
	char			*val;
	struct s_env	*next;
}					t_env;

typedef enum e_token
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
}					t_token;

typedef struct s_redir
{
	t_token			mode;
	char			*file;
	int				fd;
	struct s_redir	*next;
}					t_redir;

typedef struct s_cmd
{
	char			*cmd;
	char			**args;
	t_redir			*redirections;
}					t_cmd;

typedef struct s_list
{
	char			*s;
	int				expand_flag;
	int				amibiguous_redir;
	t_token			type;
	t_cmd			data;
	struct s_list	*sub_list;
	struct s_list	*prev;
	struct s_list	*next;
}					t_list;

typedef struct s_expand
{
	char			*str;
	int				i;
	char			*res;
	int				res_idx;
	int				res_size;
	int				quotes_flags[2];
	bool			to_split;
	bool			to_sort;
	t_list			*idx_node;
}					t_expand;

typedef struct s_wildcard
{
	char			**segments;
	char			*pattern;
	char			*ptr;
	char			*start;
	char			*curr_path;
	char			*next_path;
	int				num_segments;
	int				idx;
	int				len;
	bool			skip_hidden;
	bool			add_slash;
	bool			match_found;
	t_expand		*params;
	t_list			*node;
	t_list			*last_node;
}					t_wildcard;

typedef struct s_tree
{
	t_token			type;
	t_cmd			data;
	struct s_tree	*sub_tree;
	struct s_tree	*left;
	struct s_tree	*right;
}					t_tree;

/** STRUCTURES FUNCTIONS **/

t_list				*new_node(char *s, int type);
void				add_node(t_list **node, t_list *new_node);
t_list				*insert_node(t_list *start, t_list *new_node);
void				free_tree(t_tree *tree);
void				free_array(char **arr);
void				free_list(t_list *list);
void				print_list(t_list *list);
void				print_ast(t_tree *node, int level);

/**************************/

/** LEXER FUNCTIONS **/

void				parse_operators(t_list **list, char *s, int *i);
void				parse_words(t_list **list, char *s, int *i);
void				parse_quotes(char *s, int *i);
t_list				*lexer(char *s);
int					ft_isspace(char c);
int					is_operator(char c);

/*********************/

/** PARSER FUNCTIONS **/

void				set_position(t_tree *stats[]);
t_tree				*convert_to_ast(t_list *list);
void				parser(t_list *list);
t_list				*get_redirections(t_list *list, t_list *current,
						t_redir **redirect);
void				append_redirection(t_redir **redirection, t_redir *new);
int					get_args_count(t_list *list);
char				**extend_array(char **arr, char *new, int i, int *size);
void				merge_words(t_list *list, t_redir *redirs);
void				arrange_nodes(t_list *list[3], t_redir **redirections);

	/*-- EXPANDER FUNCTIONS --*/

void				expand_rm_quotes(t_list *node, char *s);
char				*extend_string(t_expand *params);
char				*append_value(t_expand *params, char *value);

		/**--__ env variables functions __--**/

void				expand_exit_status(t_expand *params);
char				*get_varname(char *s, int *j);
bool				innormal_var(t_expand *params);
void				split_node(t_list *node);

		/**--__wildcard functions__--**/

void				expand_wildcards(t_expand *params, t_list *node);
char				*get_pattern(t_expand *params);
void				add_match(t_wildcard *rules, char *str);
char				*construct_filename(t_wildcard *rules, char *s);
char				*get_segment(char *s, int *idx, int len);
void				set_segments(t_wildcard *rules);
void				sort_fnames(t_list *start, t_list *end);
void				swap_strings(t_list *a, t_list *b);
void				final_touches(t_wildcard *rules);

/**********************/

/** ENV AND BUILTINS FUNCTIONS **/

t_env				*create_env(char **env);
t_env				*new_env(char *s, int *status);
int					add_env(t_env **env, t_env *new_env);
void				del_env(t_env **env, char *var);
void				free_env(t_env *env);
char				*get_val(char *s, int *status);
char				*get_var(char *s, int *status);
char				*get_env(char *name, t_env *env);
int					set_env(char *var, char *val, t_env **env);
int					export_f(char **env, t_mini *mini);
int					unset_f(char **env, t_mini *mini);
int					env_f(t_env *env, int flag);
int					echo_f(char **args, t_mini *mini);
int					cd_f(char **path, t_mini *mini);
int					pwd_f(char **args, t_mini *mini);
int					exit_f(char **args, t_mini *mini);
int					cd_free(void *p, int f);

/********************************/

/** EXECUTION FUNCTIONS **/

int					execute_command(t_tree *root, t_mini *mini);
int					execute_and(t_tree *root, t_mini *mini);
int					execute_or(t_tree *root, t_mini *mini);
int					execute_parenthesis(t_tree *root, t_mini *mini);
int					execute_pipe(t_tree *node, t_mini *mini);
char				*path_join(char *path, char *cmd);
int					check_path(char *cmd, int *status);
int					join_command(t_tree *root, t_env **env, int *status);
int					check_builtin(t_tree *root, t_mini *mini, int *exit);
int					ft_strcmp(char *s1, char *s2);
void				cmd_error(char *cmd);
void				error_msg(char *msg, t_mini *mini, int status);
void				free_(char **arr);
void				free_exit(t_mini *mini, int status);
int					*create_pipe(int *pipefd, t_mini *mini);
void				close_pipe(int *pipefd);
int					end_of_pipe(pid_t pid_left, pid_t pid_right, int *pipefd);
int					close_fd(int fd1, int fd2);
int					redirections_type(t_redir *redirections, t_mini *mini);
int					check_redirection(t_tree *root, t_mini *mini);
char				**count_env(t_env *env);
void				create_char_env(char **ev, t_env *env);
char				**convert_env(t_env *env);
void				free_char_env(char **env);
int					heredoc(const char *delimiter, t_mini *mini);
int					execute_ast(t_tree *root, t_mini *mini);
void				child_process1(t_tree *root, int *pipefd, t_mini *mini);
void				child_process2(t_tree *root, int *pipefd, t_mini *mini);
void				free_mini(t_mini *mini);
t_mini				*create_mini(char **env);
void				free_and_reset(t_mini *mini);
void				read_heredoc(t_tree *root, t_mini *mini);
void				reset_fd(int fd_in, int fd_out);

/*************************/

#endif