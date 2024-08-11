/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 19:15:54 by bruno             #+#    #+#             */
/*   Updated: 2024/08/10 20:09:27 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*update_prompt(void)
{
	char	cwd[100];//check size
	char	*dir;
	char	*prompt;
	char	**folders;
	int		i;
	
	dir = getcwd(cwd, sizeof(cwd));//error check
	folders = ft_split(dir, '/');//error check
	i = 0;
	while (folders[i])
		i++;
	prompt = folders[i - 1];
	prompt = ft_strjoin(prompt, "$ ");//error check
	free_array(folders);
	return (prompt);
}

/* void print_jobs(t_jobs *jobs) 
{
    t_jobs *curr = jobs;
    while (curr != NULL) 
	{
        int i = 0;
        printf("cmd: %s  execd: %s  type: %d\n", curr->cmd, curr->execd, curr->type);
        while (curr->job && curr->job[i]) 
		{
            printf("job: %s\n", curr->job[i]);
            i++;
        }
        curr = curr->next;
    }
} */