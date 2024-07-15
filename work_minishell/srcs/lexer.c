/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:20:43 by ycantin           #+#    #+#             */
/*   Updated: 2024/07/10 22:06:25 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//create linked list of tokens -------> ex: echo "i" && cat Makefile | grep a BECOMES list: echo->i->&&->cat->makefile->|->grep->a
//DONE ^
//after doing this, do parsing similarly done in test.c

void    tokenize(t_token **list, char *str)
{
    int i;
    char **array;
    t_token *new_node;
    
    i = 0;
    array = token_array(str);
    if (!array)
        return ;
    while (array[i])
    {
        new_node = addtok(ft_strdup(array[i]));
        if (!new_node)
            free_all(list, array, "Error\n", 7);  
        new_node->pos = i;
        new_node->type = define_type(array[i]);
        go_to_next(list, new_node);
        i++;
    }
    free_array(array);
}

int     ft_quote(char *str, int i)
{
    char quote;
    
    quote = str[i];
    i++;
    while (str[i] && str[i] != quote)
        i++;
    i++;
    return (i);
}

int count_words(char *str)
{
    int i;
    int wc;
    int in_word;
    
    i = 0;
    wc = 0;
    in_word = 0;
    if (!str)
        return (0);
    while (str[i])
    {
        if (!in_word && (str[i] == '\'' || str[i] == '"'))
        {
            i = ft_quote(str, i);
            wc++;
        }
        if (!in_word && !(str[i] == ' ' || str[i] == '\t' || str[i] == '\n'))
        {
            wc++;
            in_word = 1;
        }
        if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
            in_word = 0;
        i++;
    }
    return (wc);
}

int assign_i(char *str, int i)
{
    char quote;
    
    if (str[i] == '\'' || str[i] == '"')
    {
        quote = str[i];
        i++;
        while (str[i] && str[i] != quote)
            i++;
        i++;
    }
    else
    {
        while (str[i] && !(str[i] == ' ' || str[i] == '\t' || str[i] == '\n'))
            i++;
    }
    return (i);
}

char **token_array(char *str)
{
    t_holder h;
    
    h.i = 0;
    h.j = 0;
    h.k = 0;
    h.wc = count_words(str);
    h.array = malloc (sizeof (char *) * (h.wc + 1));
    if (!h.array)
        return (NULL);
    while (str[h.i])
    {
        while (str[h.i] && (str[h.i] == ' ' || 
            str[h.i] == '\t' || str[h.i] == '\n'))
            h.i++;
        h.j = h.i;
        h.i = assign_i(str, h.i);
        if (h.i > h.j)
        {
            h.array[h.k] = ft_substr(str, h.j, h.i - h.j);
            h.k++;
        }
    }
    h.array[h.k] = NULL;
    return (h.array);
}
