/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   smooth_usrexp.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:20:43 by ycantin           #+#    #+#             */
/*   Updated: 2024/07/21 18:03:48 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//make function that calculates space for new string. 
//when a char is equal to the character we want to cehck for, it will 
//check if the char before is a space. if its not, buf size ++ else,
// iterate until weve paassed the characters we are searching for.
// if the char after the last char 
//we are searching for a space, continue. else, buf size++;
int count(char *str)
{
    int i;
    int count;
    
    i = 0;
    count = 0;
    if (!str)
        return (0);
    while (str[i])
    {
        while (str[i] == ' ' || str[i] == '\t')
            i++;
        if (str[i] == '\0')
            break ;
        if (str[i] == '&' || str[i] == '|' || str[i] == '<' || str[i] == '>' || str[i] == '=')
        {
            while (str[i] && (str[i] == '&' || str[i] == '|' || str[i] == '<' || str[i] == '>' || str[i] == '='))
            {
                count++;
                i++;
            }
            count ++;
        }
        else
        {
            while (str[i] && (str[i] != '&' && str[i] != '|' && str[i] != '<' && str[i] != '>' && str[i] != '=' && str[i] != '\t' && str[i] != ' '))
            {
                count++;
                i++;
            }
            count++;
        }
    }
    return (count);
}

char *split_complex_args(char *str)
{
    int i;
    int f;
    int size;
    char *new;

    i = 0;
    f = 0;
    size = count(str);
    new = malloc (sizeof (char) * (size + 1));
    if (!new)
        return (NULL);
    while (str[i])
    {
        while (str[i] && str[i] == ' ' || str[i] =='\t')
            i++;
        if (str[i] == '\0')
            break;
       if (str[i] == '&' || str[i] == '|' || str[i] == '<' || str[i] == '>' || str[i] == '=')
        {
            while (str[i] && (str[i] == '&' || str[i] == '|' || str[i] == '<' || str[i] == '>' || str[i] == '='))
                new[f++] = str[i++];
            new[f++] = ' ';
        }
        else
        {
            while (str[i] && (str[i] != '&' && str[i] != '|' && str[i] != '<' && str[i] != '>' && str[i] != '=' && str[i] != '\t' && str[i] != ' '))
                new[f++] = str[i++];
            new[f++] = ' ';
        }  
    }
    new[f] = '\0';
    return (new);
}

/* int main(void)
{
    char *str = split_complex_args("echo hi>>output.c&&cat makefile");
    printf("%s\n", str);
    free(str);
    return 0;
} */
