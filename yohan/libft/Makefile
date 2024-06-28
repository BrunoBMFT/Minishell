# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/24 12:26:26 by ycantin           #+#    #+#              #
#    Updated: 2024/06/28 11:22:44 by ycantin          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
# PRINTF_DIR = ft_printf
# PRINTF = $(PRINTF_DIR)/libftprintf.a

SRC = ft_memset ft_bzero ft_memcpy ft_memmove ft_memchr \
ft_memcmp ft_strlen ft_strlcpy ft_strlcat ft_strchr ft_strrchr \
ft_substr ft_putchar_fd ft_putstr_fd ft_putendl_fd ft_putnbr_fd \
ft_strnstr ft_strncmp ft_atoi ft_isalpha ft_isdigit ft_isalnum \
ft_isascii ft_isprint ft_toupper ft_tolower ft_calloc ft_strdup \
ft_strjoin ft_strtrim ft_strmapi ft_striteri ft_itoa ft_split ft_strcmp \
numbers str ft_printf ft_strcat ft_free_array \

BONUS_SRC = ft_lstnew ft_lstadd_front ft_lstsize ft_lstlast ft_lstadd_back\
	ft_lstdelone ft_lstclear ft_lstiter ft_lstmap
BONUS = $(BONUS_SRC:=.o)
SRCES = $(SRC:=.o)
CC = cc
RM = rm -f
CFLAGS = -Wall -Werror -Wextra

all:$(NAME)

# $(PRINTF): 
# 	make -C $(PRINTF_DIR)

$(NAME): $(SRCES)
	ar rc $(NAME) $(SRCES)

clean:
	$(RM) $(SRCES) $(BONUS)
# make clean -C $(PRINTF_DIR)

fclean: clean
	$(RM) $(NAME)
# make fclean -C $(PRINTF_DIR)

re:	fclean $(NAME)

bonus:	$(BONUS) $(SRCES)
	ar rc $(NAME) $?

%.o: %.c
	$(CC) -c $(CFLAGS) $?


.PHONY: all clean fclean re bonus 