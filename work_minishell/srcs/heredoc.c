/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:20:43 by ycantin           #+#    #+#             */
/*   Updated: 2024/07/21 18:02:06 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h"

void    heredoc(char *str)
{
    int fd;
    char *line;

    if ((fd = open(".heredoc", O_CREAT | O_RDWR, 0644)) < 0)
    {
        perror("heredoc misconduct");
        return ;
    }
    while (1)
    {
        line = readline("heredoc> ");
        if (ft_strcmp(str, line) == 0)
        {
            free(line);
            break ;
        }
        ft_putendl_fd(line, fd);
        free(line);
    }
    close (fd);
    return ;
}

void    start_heredoc(t_jobs *curr)
{
    int fd;
	char *line;
    
    heredoc(curr->execd);
	fd = open(".heredoc", O_RDONLY);
    while (1)
    {
        line = get_next_line(fd);
        if (!line)
            break ;
        ft_printf("%s", line);
        free(line);
    }
    close (fd);
    remove(".heredoc");
    return ;
}
