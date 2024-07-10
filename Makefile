# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: danjimen <danjimen@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/10 14:07:24 by danjimen          #+#    #+#              #
#    Updated: 2024/07/10 14:15:02 by danjimen         ###   ########.fr        #
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

SRC_DIR			=	src

SRC				=	$(SRC_DIR)/main.c

OBJ				=	$(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	$(call print_cyan, "Compiled minishell")

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJ)
	$(call print_green, "Cleaned minishell objects")

fclean: clean
	@rm -f $(NAME)
	$(call print_green, "Cleaned minishell executable")

re: fclean all

.PHONY: all clean fclean re
