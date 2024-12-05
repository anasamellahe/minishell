NAME = libft.a
CC = cc
CFLAGS = -Werror -Wextra -Wall

SRCS = ft_putnbr_fd.c ft_putendl_fd.c ft_putstr_fd.c ft_putchar_fd.c \
    ft_striteri.c ft_strmapi.c ft_itoa.c ft_split.c ft_strtrim.c \
    ft_strjoin.c ft_substr.c ft_strdup.c ft_calloc.c ft_atoi.c \
    ft_strnstr.c ft_memcmp.c ft_memchr.c ft_strncmp.c ft_strrchr.c \
    ft_strchr.c ft_tolower.c ft_toupper.c ft_strlcat.c ft_strlcpy.c \
    ft_memmove.c ft_memcpy.c ft_bzero.c ft_memset.c ft_strlen.c \
    ft_isprint.c ft_isascii.c ft_isalnum.c ft_isdigit.c ft_isalpha.c ft_strncpy.c \

OBJS_DIR = libft_objs
OBJS = $(addprefix $(OBJS_DIR)/, $(SRCS:.c=.o))

ERASE := \033[2K
CYAN := \033[36m
MAGENTA	:= \033[35m
BOLD := \033[1m
END := \033[0m

.SILENT:

all: $(NAME)

$(OBJS_DIR)/%.o: %.c
	@mkdir -p $(OBJS_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@
	@printf "$(ERASE)$(CYAN) > Compilation :$(END) $<\r"

$(NAME): $(OBJS)
	@ar rcs $(NAME) $(OBJS)
	@printf "$(CYAN)=> $(NAME) made successfully.$(END)\n"

clean:
	@rm -f $(OBJS) $(BOBJS)
	@rm -rf $(OBJS_DIR)
	@printf "$(CYAN)=> removed libft_objs successfully.$(END)\n"

fclean: clean
	@rm -f $(NAME)
	@printf "$(CYAN)=> removed $(NAME) successfully.$(END)\n"

re: fclean all


.PHONY: all clean fclean re bonus
.SECONDARY:	$(OBJS)