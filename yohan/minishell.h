/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 16:38:21 by ycantin           #+#    #+#             */
/*   Updated: 2024/06/28 17:03:48 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL
# define MINISHELL

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "./libft/libft.h"

typedef struct s_holder
{
    int i;
    int j;
    int wc;
    int k;
    char **array;
}t_holder;

typedef struct s_token
{
    char            *token;
    int             pos;
    int             type;
    struct  s_token   *prev;
    struct  s_token   *next;
}t_token;

typedef struct s_ast
{
    int              type;
    int              placement;
    struct s_ast     *next;
}t_ast;

//free:
void	clear_list(t_token **lst);

char        **token_array(char *str);
void        tokenize(t_token **list, char *str);

void	    free_all(t_token **list, char **array, char *message, int len);
//lexer:
t_token	    *addtok(void *content);
t_token	    *get_last_tok(t_token *lst);
void	    go_to_next(t_token **lst, t_token *new);

#endif