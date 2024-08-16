/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:20:43 by ycantin           #+#    #+#             */
/*   Updated: 2024/08/16 00:58:51 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h"

//add heredoc signal handling

// void    heredoc(char *str)
// {
//     int fd;
//     char *line;

//     if ((fd = open(".heredoc", O_CREAT | O_RDWR, 0644)) < 0)
//     {
//         perror("heredoc misconduct");
//         return ;
//     }
//     while (1)
//     {
//         line = readline("heredoc> ");
//         if (ft_strcmp(str, line) == 0)
//         {
//             free(line);
//             break ;
//         }
//         ft_putendl_fd(line, fd);
//         free(line);
//     }
//     close (fd);
// }

// void    start_heredoc(t_jobs *curr)
// {
//     int fd;
// 	char *line;
    
//     heredoc(curr->input);
// 	fd = open(".heredoc", O_RDONLY);
//     if (fd < 0)
//     {
//         perror("error opening heredoc fd");
//         return ;
//     }
//     if (dup2(fd, STDIN_FILENO) < 0)
//        perror("heredoc dup2 error");
//     print_file(fd);
//     close (fd);
// }

void handle_heredoc(char *delimiter)
{
    int fd;
    char *line;
    
    fd = open(".heredoc", O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (fd < 0)
    {
        perror("heredoc error opening file");
        return;
    }
    while ((line = readline("heredoc> ")) != NULL)
    {
        if (ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        ft_putendl_fd(line, fd);
        free(line);
    }
    if (dup2(fd, STDIN_FILENO) < 0)
        perror("heredoc error duplicating fd");
    close(fd);
}
