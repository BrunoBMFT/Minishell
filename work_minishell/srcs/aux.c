/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 19:15:54 by bruno             #+#    #+#             */
/*   Updated: 2024/08/01 17:07:59 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//temp_vars should be an exact copy of env, and then when vars are exported, it should place them in env.
//to check if a new variable was declared, it should do a strncmp till = sign to see if there is a variable with that name
char	**variable_declaration(t_jobs *job, char **temp_vars, char **env)//problem when declaring 3 variables
{
	if (ft_strnstr(job->cmd, "=", ft_strlen(job->cmd)))
	{
		char	**strs = ft_split(job->job[0], ' ');//error check
		temp_vars = add_to_env(strs, temp_vars);//error check
		if (job->job[1])
		{
			char	**strs = ft_split(job->job[1], ' ');//error check
			temp_vars = add_to_env(strs, temp_vars);//error check
		}
	}
	if (temp_vars)
		return (temp_vars);
	return (NULL);
}

void print_jobs(t_jobs *jobs) 
{
    t_jobs *curr = jobs;
    while (curr != NULL) 
	{
        int i = 0;
        printf("cmd: %s  execd: %s  type: %d\n", curr->cmd, curr->execd, curr->type);
/*         while (curr->job && curr->job[i]) 
		{
            printf("job: %s\n", curr->job[i]);
            i++;
        } */
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
