/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:20:43 by ycantin           #+#    #+#             */
/*   Updated: 2024/07/21 18:03:33 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//goal-> loop over all jobs in order to find redirections. if > is found, 

void    inspect(t_jobs **job)
{
    if ((*job)->type == WORD)
        *job = (*job)->next;
    else if((*job)->type == AND)
        job_and(&job);
    else if ((*job)->type == OR)
        job_or(&job);
    else if ((*job)->type == PIPE)
        job_bruno(&job);
    else if ((*job)->type == INPUT)
        job_redir_input(&job);
    else if ((*job)->next->type == OUTPUT)
        job_redir_output(&job);
    else if ((*job)->next->type == APPEND_OUT)
        append_to_file(&job);
}
//current_file is the cmd  of node->next->next and execd is th execd of the output of the last command
int    append_to_file(t_jobs **node) 
{
    int fd;

    if (access((*node)->next->next->cmd, F_OK) != 0)
        fd = open((*node)->next->next->cmd, O_CREAT | O_RDWR, 0644);
    else
        fd = open((*node)->next->next->cmd, O_APPEND | O_RDWR);
    if (fd < 0)
        perror("appending file error");
    //ft_putendl_fd((*node)->execd, fd);
    *node = (*node)->next->next;
    return (fd);
}
