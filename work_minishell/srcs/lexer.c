/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:20:43 by ycantin           #+#    #+#             */
/*   Updated: 2024/07/21 17:24:44 by bruno            ###   ########.fr       */
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
    t_holder lst;
    
    lst.i = 0;
    lst.j = 0;
    lst.k = 0;
    lst.wc = count_words(str);
    lst.array = malloc(sizeof (char *) * (lst.wc + 1));
    if (!lst.array)
        return (NULL);
    while (str[lst.i])
    {
        while (str[lst.i] && (str[lst.i] == ' ' || 
            str[lst.i] == '\t' || str[lst.i] == '\n'))
            lst.i++;
        lst.j = lst.i;
        lst.i = assign_i(str, lst.i);
        if (lst.i > lst.j)
        {
            lst.array[lst.k] = ft_substr(str, lst.j, lst.i - lst.j);
            lst.k++;
        }
    }
    lst.array[lst.k] = NULL;
    return (lst.array);
}
