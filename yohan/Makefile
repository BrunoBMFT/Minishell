# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/23 16:39:56 by ycantin           #+#    #+#              #
#    Updated: 2024/06/28 16:57:17 by ycantin          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a
NAME = minishell

CC = cc
FLAGS = -Wall -Werror -Wextra -g
DEPS = -lreadline
RM = rm -f
SRC = 	main.c lexer/lexer.c lexer/lexer_utils.c frees.c \

all: $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)

$(NAME): $(LIBFT) $(SRC)
	@$(CC) $(FLAGS) $(SRC) $(LIBFT) -o $(NAME) $(DEPS)
	
clean:
	@make clean -C $(LIBFT_DIR)

fclean:
	@$(RM) $(NAME)
	@make fclean -C $(LIBFT_DIR)

re: fclean all
