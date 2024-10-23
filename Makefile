# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: danjimen <danjimen@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/10 14:07:24 by danjimen          #+#    #+#              #
#    Updated: 2024/10/23 20:47:56 by danjimen         ###   ########.fr        #
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
MAIN_DIR		=	src/main
BUILTINS_DIR	=	src/built-ins
PARSE_DIR		=	src/parse
ENV_DIR			=	src/env
SRC				=	$(MAIN_DIR)/main.c \
					$(MAIN_DIR)/errors_and_exit.c \
					$(MAIN_DIR)/signals_and_history.c \
					$(MAIN_DIR)/initialize_main.c \
					$(SRC_DIR)/list_of_params.c \
					$(SRC_DIR)/here_doc.c \
					$(SRC_DIR)/new_red_exe.c \
					$(SRC_DIR)/find_redirections.c \
					$(SRC_DIR)/execution_matrix.c \
					$(SRC_DIR)/execution_structure.c \
					$(SRC_DIR)/redirector.c \
					$(SRC_DIR)/start_executing.c \
					$(SRC_DIR)/find_path.c \
					$(SRC_DIR)/execute_commands.c \
					$(SRC_DIR)/execution_utils.c \
					$(BUILTINS_DIR)/ft_echo.c \
					$(BUILTINS_DIR)/ft_exit.c \
					$(BUILTINS_DIR)/ft_export.c \
					$(BUILTINS_DIR)/ft_export_utils.c \
					$(BUILTINS_DIR)/ft_unset.c \
					$(BUILTINS_DIR)/ft_pwd.c \
					$(BUILTINS_DIR)/ft_cd.c \
					$(BUILTINS_DIR)/ft_env.c \
					$(PARSE_DIR)/parse.c \
					$(PARSE_DIR)/ft_tokenize.c \
					$(PARSE_DIR)/ft_retokenize.c \
					$(PARSE_DIR)/tokenize_out_of_quotes.c \
					$(PARSE_DIR)/expander.c \
					$(PARSE_DIR)/expander_utils.c \
					$(ENV_DIR)/ft_list_env.c \
					$(ENV_DIR)/ft_env.c \
					$(ENV_DIR)/ft_env_unset.c

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
	$(call print_cyan,"Compiled minishell")

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(MAKE) -s -C $(LIBFT_DIR) clean
	@rm -f $(OBJ)
	$(call print_green,"Cleaned minishell objects")

fclean: clean
	@$(MAKE) -s -C $(LIBFT_DIR) fclean
	@rm -f $(NAME)
	$(call print_green,"Cleaned the library libft.a")
	$(call print_green,"Cleaned minishell executable")

re: fclean all

.PHONY: all clean fclean re
