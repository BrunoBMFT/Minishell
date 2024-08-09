/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:20:43 by ycantin           #+#    #+#             */
/*   Updated: 2024/08/08 02:50:31 by bruno            ###   ########.fr       */
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
    }
}

int    update_input(t_jobs *job)
{
	int	fd;

	fd = open(job->execd, O_RDONLY);
	if (fd < 0)
	{
		perror(job->execd);//print minishell: first (how bash handles it)
		close (fd);
		return (-1);
	}
	if (job->next)
	{
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			perror("dup2 error input redir\n");
			clean_exit(job, NULL, NULL);//return something
		}
	}
	else
		print_file(fd);
	close (fd);
    return (0);
}

int update_output(t_jobs *job, char **env, char **temp_vars)
{
    int fd;
    int status;
    int stdout;

    stdout = dup(STDOUT_FILENO);
    if (access(job->next->execd, F_OK) == 0)
        remove(job->next->execd);
    fd = open(job->next->execd, O_CREAT | O_RDWR, 0644);
    if (fd < 0)
        perror("error during output redir\n");
    if (dup2(fd, STDOUT_FILENO) < 0)
    {
        perror("dup2 error output redir\n");
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

int    append_to_file(t_jobs *job, char **env, char **temp_vars) 
{
    int fd;
    int status;
    int stdout;

	stdout = dup(STDOUT_FILENO);
/*     if (access(job->next->execd, F_OK) != 0)
        fd = open(job->next->execd, O_CREAT | O_RDWR, 0644);
    else
        fd = open(job->next->execd, O_APPEND | O_RDWR); */
    fd = open(job->next->execd, O_CREAT | O_APPEND | O_RDWR, 0644);
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