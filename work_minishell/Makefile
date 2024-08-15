# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bruno <bruno@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/23 16:39:56 by ycantin           #+#    #+#              #
#    Updated: 2024/08/15 20:45:46 by bruno            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =	minishell

SRCS_DIR = ./srcs
SRCS =	main.c aux.c builtins.c executor.c process.c process_aux.c \
		heredoc.c signal.c smooth_usrexp.c \
		lexer.c lexer_utils.c frees.c job_list.c job_utils.c \
		parse.c expansion.c redirections.c

OBJS_DIR = ./objs
OBJS = $(addprefix $(OBJS_DIR)/, $(SRCS:.c=.o))
$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@mkdir -p $(OBJS_DIR)
	@cc $(CFLAGS) -c $< -o $@

LIBFT_DIR = ./includes/libft
LIBFT = $(LIBFT_DIR)/libft.a

FLAGS = -Wall -Werror -Wextra -g
DEPS = -lreadline
RM = rm -f

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	@cc $(FLAGS) -o $(NAME) $(OBJS) $(LIBFT) $(DEPS)
	@echo $(NAME) ready

$(LIBFT):
	@echo compiling
	@make -C $(LIBFT_DIR) --no-print-directory

clean:
	@rm -rf $(OBJS_DIR)
	@make -C $(LIBFT_DIR) clean --no-print-directory
	@echo clean

fclean: clean
	@rm -rf $(NAME)
	@make -C $(LIBFT_DIR) fclean --no-print-directory
	@echo clean

re: fclean all

run: all
	clear && ./minishell
vrun: all
	clear && valgrind --leak-check=full --suppressions=readline.supp ./minishell
fullvrun: all
	clear && valgrind --leak-check=full --suppressions=readline.supp --show-leak-kinds=all --track-origins=yes ./minishell

.PHONY: all fclean clean re
