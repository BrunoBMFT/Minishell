/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 19:15:54 by bruno             #+#    #+#             */
/*   Updated: 2024/07/11 17:30:35 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	panic(char *s)
{
	ft_putendl_fd(s, 2);
	exit(1);
}

int	new_fork(void)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		panic("fork");
	return (pid);
}


char	*update_prompt()
{
	char	cwd[100];
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

t_jobs *build(char *command_line)
{
	t_jobs *jobs;
	t_token *list;

	list = NULL;
	jobs = NULL;
	tokenize(&list, command_line);
	make_job_list(&jobs, &list);
	clear_list(&list);
	return (jobs);
}