# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vafernan <vafernan@42student.fr>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/20 09:29:35 by vafernan          #+#    #+#              #
#    Updated: 2024/10/20 09:29:38 by vafernan         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Program name
NAME = termsync

# Compilation
CC = cc
CFLAGS = -Wall -Wextra -Werror -g

# Directories
TERMSYNC_DIR = TermSync

# Source and object files
SRC = $(wildcard $(TERMSYNC_DIR)/*.c)
OBJ = $(SRC:.c=.o)

# ANSI color codes
RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[1;33m
BLUE = \033[1;34m
MAGENTA = \033[1;35m
CYAN = \033[1;36m
RESET = \033[0m

# Rules
all: welcome_message $(NAME)

welcome_message:
    @echo "$(BLUE)---------------------------------------------$(RESET)"
    @echo "$(CYAN)Welcome to TermSync!$(RESET)"
    @echo "$(YELLOW)Building TermSync...$(RESET)"
    @echo "$(BLUE)---------------------------------------------$(RESET)"

$(NAME): $(OBJ)
    @$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
    @echo "$(GREEN)Build complete!$(RESET)"

$(TERMSYNC_DIR)/%.o: $(TERMSYNC_DIR)/%.c
    @$(CC) $(CFLAGS) -c $< -o $@

clean:
    @rm -rf $(OBJ)

fclean: clean
    @rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re