/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 19:15:54 by bruno             #+#    #+#             */
/*   Updated: 2024/07/24 22:24:48 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void print_jobs(t_jobs *jobs) 
{
    t_jobs *curr = jobs;
    while (curr != NULL) 
	{
        int i = 0;
        printf("cmd: %s  execd: %s  type: %d\n", curr->cmd, curr->execd, curr->type);
        while (curr->job && curr->job[i]) 
		{
            printf("->%s\n", curr->job[i]);
            i++;
        }
        curr = curr->next;
    }
}

void	check_exit(char *line)// wrong for cmd1 | exit
{
	if (ft_strcmp(line, "exit") == 0)
	{
		free(line);
		rl_clear_history();
		exit(0);
	}
}


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

int	clear_proc(char **env)// * REMOVE
{
	int	pid = new_fork();
 	if (pid == 0)
	{
		char *const com[] = { "clear", NULL };
		execve("/bin/clear", com, env);
		panic("clear fail\n");//free fds, show exit code and perror
	}
	int status = 0;
	return (0);
}
