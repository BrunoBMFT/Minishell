/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brfernan <brfernan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 19:15:54 by bruno             #+#    #+#             */
/*   Updated: 2024/08/21 18:54:51 by brfernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*update_prompt(void)//make better
{
	char	cwd[PATH_MAX];
	char	*dir;
	char	*prompt;
	char	**folders;
	int		i;

	getcwd(cwd, PATH_MAX);//error check clean exit
	folders = ft_split(cwd, '/');
	if (!folders)
		return (NULL);//free folders
	i = 0;
	while (folders[i])
		i++;
	prompt = ft_strjoin(folders[i - 1], "$ ");//error check
	free_array(folders);
	if (!prompt)
		return (free_array(folders), NULL);
	return (prompt);
}

void print_jobs(t_jobs *jobs)/*to remove*/
{
	t_jobs	*curr;
	int		i = 0;

	curr = jobs;
	while (curr != NULL) {
		int i = 0;
		while (curr->job && curr->job[i]) {
			printf("job %d: %s\n", i, curr->job[i]);
			i++;
		}
		curr = curr->next;
	}
}

char	**dup_envp(char **envp)//maybe put this one?
{
	//have this tell if env -i has happened, print funny message
	char	**new_env;
	char	cwd[PATH_MAX];
	int		i;

	new_env = envp;
	i = 0;
	while (envp[i])//error check
	{
		if (ft_strncmp(envp[i], "SHELL=", 6) == 0)
			break ;
		i++;
	}
	getcwd(cwd, PATH_MAX);//error check
	new_env[i] = ft_calloc(sizeof(char), ft_strlen(cwd));//error check
	new_env[i] = ft_strjoin("SHELL=", cwd);
	return (new_env);
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