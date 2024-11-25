# Colors for output
CYAN		:=	\033[36m
BOLD		:=	\033[1m
GREEN		:=	\033[32m
MAGENTA		:=	\033[35m
END			:=	\033[0m

# Compiler and flags
CC = cc
CFLAGS = -Wall -Werror -Wextra  -g #-fsanitize=address -g3
# MAKEFLAGS += --no-print-directory
# Directories
OBJS_DIR = objs

# Source files
LEXER_SRCS = LEXER/lexer.c LEXER/node.c LEXER/utils.c
PARSER_SRCS = PARSER/expand.c PARSER/expand_utils.c PARSER/expand_utils2.c PARSER/parser.c PARSER/ft_free.c\
			PARSER/tree.c PARSER/wildcard.c PARSER/wildcard_utils.c PARSER/parser_utils.c
EXECUTER_SRCS = BUILTIN/cd_f.c BUILTIN/echo_f.c BUILTIN/env_f.c BUILTIN/env_node.c BUILTIN/pwd_f.c BUILTIN/export_f.c BUILTIN/unset_f.c BUILTIN/utils.c BUILTIN/utils1.c BUILTIN/exit_f.c\
				EXECUTION/exec.c EXECUTION/command_utils.c EXECUTION/error_utils.c EXECUTION/exec_utils.c EXECUTION/pipe_utils.c EXECUTION/redirections_utils.c EXECUTION/convert_env_utils.c EXECUTION/create_mini.c\
				main.c

SRCS = $(LEXER_SRCS) $(PARSER_SRCS) $(EXECUTER_SRCS)

# Object files
OBJS = $(patsubst %.c, $(OBJS_DIR)/%.o, $(SRCS))

# Output executable
NAME = minishell

# Libft
LIBFTPATH = libft
LIBFT = $(LIBFTPATH)/libft.a

# Targets
all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@$(CC) $(OBJS) $(CFLAGS) -lreadline $(LIBFT) -o $@
	@printf "$(GREEN)$(BOLD)> $(NAME) made successfully.$(END)\n"

$(OBJS_DIR)/%.o: %.c Makefile includes/minishell.h
	@mkdir -p $(OBJS_DIR)
	@mkdir -p $(OBJS_DIR)/LEXER $(OBJS_DIR)/PARSER $(OBJS_DIR)/BUILTIN $(OBJS_DIR)/EXECUTION
	@$(CC) $(CFLAGS) -c $< -o $@ 
	@printf "$(CYAN) > Compilation :$(END) $<\r"

$(LIBFT): $(LIBFTPATH)
	@make -C $(LIBFTPATH)

clean:
	@make -C $(LIBFTPATH) clean 
	@rm -rf $(OBJS_DIR)
	@printf "$(CYAN)=> removed objs successfully.$(END)\n"

fclean: clean
	@rm -f $(NAME)
	@rm -f $(LIBFT)
	@printf "$(CYAN)=> removed $(LIBFT) successfully.$(END)\n"
	@printf "$(CYAN)=> removed $(NAME) successfully.$(END)\n"

re: fclean all

.PHONY: all libft clean fclean re
# .SILENT:
