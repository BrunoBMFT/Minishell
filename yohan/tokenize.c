/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:20:43 by ycantin           #+#    #+#             */
/*   Updated: 2024/06/28 12:42:02 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//create linked list of tokens -------> ex: echo "i" && cat Makefile | grep a BECOMES list: echo->i->&&->cat->makefile->|->grep->a

//after doing this, do parsing similarly done in test.c
typedef struct s_token
{
    char            *token;
    int             pos;
    int             type;
    struct  s_token   *prev;
    struct  s_token   *next;
}t_token;

int count_words(char *str)
{
    int i;
    int wc;
    int in_word;
    
    i = 0;
    wc = 0;
    in_word = 0;
    while (str[i])
    {
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

char *handle_quotes(char *str, int start)
{
    int i;

    i = 0;
    if (str[start] == 34)
    {
        i = start;
        while (str[start] && str[start] != 34);
        start++;
        return()
    }
    
}

char **token_array(char *str)
{
    int i;
    int j;
    int wc;
    int k;
    char **array;
    
    i = 0;
    j = 0;
    k = 0;
    wc = count_words(str);
    array = malloc (sizeof (char *) * (wc + 1));
    if (!array)
        return (NULL);
    while (str[i])
    {
        if (str[i] == 34 || str[i] == 39)
        {
            array[k] = handle_quotes(str, i);
            k++;
        }
        while (str[i] && (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'))
            i++;
        j = i;
        while (str[i] && !(str[i] == ' ' || str[i] == '\t' || str[i] == '\n'))
            i++;
        if (i > j)
            array[k] = ft_substr(str, j, i - j);
        k++;
    }
    array[k] = NULL;
    return (array);
}

void    tokenize(t_token *list, char *str)
{
    int i;
    char **array;
    
    i = 0;
    array = token_array(str);
    while (array[i])
    {
        list->token = ft_lstnew(array[i]);
        list->pos = i;
        if (ft_strcmp(array[i], "||") == 0 || ft_strcmp(array[i], "&&") == 0)
            list->type = 1;
        else
            list->type = 0;
        i++;
        list = list->next;
    }
    free_array(array);
}

int main(void)
{
    char *str = "echo 'hi' && cat makefile | wc -l";
    t_token *list;
    tokenize(list, str);
    t_token *cur = list;
    while (cur)
    {
        printf("%s\n", cur->token);
        cur = cur->next;
    }
    // char **array = token_array(str);
    // printf("%d\n\n", count_words(str));
    // int i = 0;
    // while (array[i])
    // {
    //     printf("%s\n", array[i]);
    //     free(array[i]);
    //     i++;
    // }
    //free(array);
}