/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:20:43 by ycantin           #+#    #+#             */
/*   Updated: 2024/08/19 02:05:38 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void    print_file(int fd)
{
    char *line;
    
    while (1)
    {
        if (!(line = get_next_line(fd)))
            break ;
        ft_printf("%s", line);
        free(line);
    }
}

void    update_input(t_jobs *job)
{
    int fd;
    int i;
    
    fd = open(job->job[0], O_RDONLY);
    
    if (fd < 0)
    {
        ft_printf("No such file or directory:  %s\n", job->job[0]);
        return ;
    }
    if (job->next)
    {
        if (dup2(fd, STDIN_FILENO) == -1)
        {
            perror("dup2 error input redir\n");
            return ;
        }
    }
    // else
    //     print_file(fd);
    close (fd);
}

int update_output(t_jobs *job, char **env, char ***temp_vars)
{
    int fd;
    int status;
    int stdout;

    stdout = dup(STDOUT_FILENO);
    if (access(job->next->job[0], F_OK) == 0)
        remove(job->next->job[0]);
    fd = open(job->next->job[0], O_CREAT | O_RDWR, 0644);
    if (fd < 0)
        perror("error during output redir\n");
    if (dup2(fd, STDOUT_FILENO) < 0)
    {
        perror("dup2 error output redir\n");
        clean_exit(job, NULL, NULL);
    }
    close (fd);
    if (job->type < INPUT && job != NULL)
        status = simple_process(job, env, temp_vars);
	if (dup2(stdout, STDOUT_FILENO) < 0)
    {
        perror("dup2 error append redir\n");
        clean_exit(job, NULL, NULL);
    }
    close (stdout);
    return (status);
}

int    append_to_file(t_jobs *job, char **env, char ***temp_vars) 
{
    int fd;
    int status;
    int stdout;

	stdout = dup(STDOUT_FILENO);
    fd = open(job->next->job[0], O_CREAT | O_APPEND | O_RDWR, 0644);
    if (fd < 0)
        perror("appending file error");
    if (dup2(fd, STDOUT_FILENO) < 0)
    {
        perror("dup2 error append redir\n");
        clean_exit(job, NULL, NULL);
    }
    close (fd);
    if (job->type < INPUT)
        status = simple_process(job, env, temp_vars);
	if (dup2(stdout, STDOUT_FILENO) < 0)
    {
        perror("dup2 error append redir\n");
        clean_exit(job, NULL, NULL);
    }
    close (stdout);
    return (status);
}