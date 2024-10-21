# Compiler and flags
CC = cc
CFLAGS = -Wall -Werror -Wextra

# Source files
SRCS =	LEXER/lexer.c LEXER/node.c LEXER/utils.c\
        PARSER/parser.c PARSER/expand.c PARSER/expand_utils.c PARSER/parser_utils.c PARSER/wildcard_utils.c PARSER/wildcard.c\
        ENV/env_node.c ENV/env.c ENV/utils.c main.c

# Object files
OBJS = $(SRCS:.c=.o)

# Output executable
NAME = minishell

# Libft directory and library
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

# Targets
all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -o $@

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all