# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: brfernan <brfernan@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/23 16:39:56 by ycantin           #+#    #+#              #
#    Updated: 2024/09/05 15:41:19 by brfernan         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =	minishell

SRCS_DIR = ./srcs
SRCS = $(addprefix aux/, $(AUX_SRCS)) $(addprefix lexer/, $(LEXER_SRCS)) $(addprefix expand/, $(EXPAND_SRCS)) $(addprefix executor/, $(EXECUTOR_SRCS)) $(addprefix builtins/, $(BUILTINS_SRCS))

AUX_SRCS = main.c aux.c frees.c signal.c parse.c
EXPAND_SRCS = expansion.c error_correction.c error_correction_utils.c 
LEXER_SRCS = lexer.c lexer_utils.c tokenize.c tokenize_utils.c
EXECUTOR_SRCS = executor.c process.c process_aux.c heredoc.c
BUILTINS_SRCS = builtins.c builtins_aux.c pwd.c env.c unset.c cd.c
#redirections.c

OBJS_DIR = ./objs
OBJS = $(addprefix $(OBJS_DIR)/, $(SRCS:.c=.o))
$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
#	@mkdir -p $(OBJS_DIR)
	@mkdir -p $(dir $@)
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

fullvrunenv: all
	clear && env -i valgrind --leak-check=full --suppressions=readline.supp --show-leak-kinds=all --track-origins=yes ./minishell
fullvrununset: all
	clear && unset PATH && /usr/bin/valgrind --leak-check=full --suppressions=readline.supp --show-leak-kinds=all --track-origins=yes ./minishell


.PHONY: all fclean clean re
