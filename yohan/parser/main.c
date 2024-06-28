/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 16:43:23 by ycantin           #+#    #+#             */
/*   Updated: 2024/06/28 12:07:58 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int main (void)
{
    char *line;
    char *dir;
    char *prompt;
    while (1)
    {
        dir = getcwd(NULL, 0);
        prompt = ft_strjoin(dir, "$ ");
        free(dir);
        line = readline(prompt);
        free(prompt);
        //tokenize(line);
        if (ft_strcmp(line, "exit") == 0)
        {
            free(line);
            break;
        }
    }
    return (0);
}
