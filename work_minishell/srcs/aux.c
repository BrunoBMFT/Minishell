/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 19:15:54 by bruno             #+#    #+#             */
/*   Updated: 2024/09/03 19:58:30 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_getpid(void)//does this work>
{
	FILE *fp;
	int	pid;
	fp = fopen("/proc/self/stat", "r");
	fp = NULL;
	if (!fp)
		return (ft_putendl_fd("minishell: getpid() error", 2), 0);
	fscanf(fp, "%d", &pid);
	fclose(fp);
	return (pid);
}

char	*update_prompt(void)//make better
{
	char	cwd[PATH_MAX];
	char	*dir;
	char	*prompt;
	char	**folders;
	int		i;

	getcwd(cwd, PATH_MAX);//error check clean exit
	if (!*cwd)
		return (ft_putendl_fd("getcwd() error", 2), NULL);
	folders = ft_split(cwd, '/');
	if (!folders)
		return (free (folders), NULL);
	i = 0;
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
	int		i;

	if (!envp || !envp[0])
		return (NULL);//printf something
	new_env = ft_calloc(sizeof(char *), ft_split_wordcount(envp) + 2);
	if (!new_env)
		return (NULL);//free new_env?
	i = 0;
	while (envp[i] && envp[i])
	{
		if (ft_strncmp(envp[i], "SHLVL=", 6) == 0)
			new_env[i] = ft_strjoin("SHLVL=", ft_itoa(ft_atoi(envp[i] + 6) + 1));//error check
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

/* void print_jobs(t_jobs *jobs)
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
} */
