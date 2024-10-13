/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 19:15:54 by bruno             #+#    #+#             */
/*   Updated: 2024/10/13 17:21:39 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//remove
void	memory_size(char **array, char *str, t_env *env, t_jobs *job)
{
	int count = 0;

	if (array)
	{
		int i = 0;
		while (array[i])
		{
			int j = 0;
			while (array[i][j])
			{
				count++;
				j++;
			}
			i++;
		}
	}
	else if (str)
	{
		int i = 0;
		while (str[i])
		{
			count++;
			i++;
		}
	}
	else if (env->env)
	{
		int i = 0;
		while (env->env[i])
		{
			int j = 0;
			while (env->env[i][j])
			{
				count++;
				j++;
			}
			i++;
		}
	}
/* void	print_jobs(char *line, t_jobs *jobs)
{
	printf("line: %s\n", line);
	int i = 0;
	while (jobs->job[i])
	{
		ft_printf_fd(2, "job %d: %s\n", i, jobs->job[i]);
		i++;
	}
} */
	else if (job->job)
	{
		while (job->job)
		{
			int i = 0;
			while (job->job[i])
			{
				int j = 0;
				while (job->job[i][j])
				{
					count++;
					j++;
				}
				i++;
			}
			job = job->next;
		}
	}
	printf("memory: %d\n", count);

	return;
}


int	ft_getpid(void)//does this work?
{
	FILE *fp;
	int	pid;

	fp = fopen("/proc/self/stat", "r");
	if (!fp)
		return (ft_putendl_fd("minishell: getpid() error", 2), 0);
	fscanf(fp, "%d", &pid);
	fclose(fp);
	return (pid);
}

char	*update_prompt(void)//void
{
	char	cwd[PATH_MAX];
	char	*dir;
	char	*prompt;
	char	**folders;
	int		i;

	getcwd(cwd, PATH_MAX);
	if (!*cwd)
		return (ft_putendl_fd("getcwd() error", 2), NULL);
	folders = ft_split(cwd, '/');
	if (!folders)
		return (free(folders), NULL);
	i = 0;
	if (!folders[i])
		return (free_array(folders), ft_strdup("/$ "));//strdup because env frees this return
	while (folders[i])
		i++;
	prompt = ft_strjoin(folders[i - 1], "$ ");
	free_array(folders);
	if (!prompt)
		return (NULL);
	return (prompt);
}

char	**dup_env(char **envp)//error check
{
	char	**new_env;
	char	*temp;
	int		i;

	i = 0;
	new_env = ft_calloc(sizeof(char *), ft_split_wordcount(envp) + 2);
	if (!new_env || !envp || !envp[0])
		return (NULL);//free new_env?
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "SHLVL=", 6) == 0)
		{
			temp = ft_itoa(ft_atoi(envp[i] + 6) + 1);//error check
			new_env[i] = ft_strjoin("SHLVL=", temp);//error check
			free (temp);
		}
		else
			new_env[i] = ft_strdup(envp[i]);//error check
		if (!new_env[i])
		{
			i = 0;
			while (new_env[i])
				free (new_env[i++]);//check if works
			return (NULL);
		}
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

t_env	init_env(char **envp)
{
	t_env	env;

	env.prompt = NULL;
	env.status = 0;
	env.env = dup_env(envp);
	return (env);
}
