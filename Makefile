# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: danjimen & isainz-r <danjimen & isainz-    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/10 14:07:24 by danjimen          #+#    #+#              #
#    Updated: 2024/09/05 15:07:45 by danjimen &       ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Colors definition
COLOR_CYAN := "\033[36m"
COLOR_GREEN := "\033[32m"
COLOR_RESET := "\033[0m"

# Macro to print a message in cyan color
define print_cyan
	@echo $(COLOR_CYAN)$1$(COLOR_RESET)
endef

# Macro to print a message in green color
define print_green
	@echo $(COLOR_GREEN)$1$(COLOR_RESET)
endef

NAME			=	minishell

CC				=	cc
CFLAGS			=	-Wall -Wextra -Werror -g3
READFLAG		=	-lreadline

SRC_DIR			=	src
BUILTINS_DIR	=	src/built-ins
SRC				=	$(SRC_DIR)/main.c \
					$(SRC_DIR)/parse.c \
					$(SRC_DIR)/ft_tokenize.c \
					$(SRC_DIR)/ft_env.c \
					$(SRC_DIR)/ft_list_env.c \
					$(SRC_DIR)/expander.c \
					$(SRC_DIR)/list_of_params.c \
					$(SRC_DIR)/executor.c \
					$(SRC_DIR)/redirector.c \
					$(SRC_DIR)/create_pipes.c \
					$(SRC_DIR)/get_redirections.c \
					$(SRC_DIR)/here_doc.c
					$(BUILTINS_DIR)/ft_built_ins.c \
					$(BUILTINS_DIR)/ft_echo.c \
					$(BUILTINS_DIR)/ft_exit.c

OBJ				=	$(SRC:.c=.o)

LIBFT_DIR	=	include/libft
LIBFT		=	$(LIBFT_DIR)/libft.a

all: $(NAME)

r: all clean
	./$(NAME)

$(LIBFT):
	@$(MAKE) -s -C $(LIBFT_DIR)

$(NAME): $(LIBFT) $(OBJ)
	@$(CC) $(CFLAGS) -I$(LIBFT_DIR) $(OBJ) $(LIBFT) $(READFLAG) -o $(NAME)
	$(call print_cyan, "Compiled minishell")

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(MAKE) -s -C $(LIBFT_DIR) clean
	@rm -f $(OBJ)
	$(call print_green, "Cleaned minishell objects")

fclean: clean
	@rm -f $(LIBFT)
	@rm -f $(NAME)
	$(call print_green, "Cleaned the library libft.a")
	$(call print_green, "Cleaned minishell executable")

re: fclean all

.PHONY: all clean fclean re
