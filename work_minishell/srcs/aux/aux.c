/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 19:15:54 by bruno             #+#    #+#             */
/*   Updated: 2024/10/25 03:57:13 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
//getpid
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
//prompt
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


//env
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
	env.env = NULL;
	if (!envp || !envp[0])
	{
		env.env = malloc(sizeof (char *));
		if (!env.env)
			return (ft_printf_fd(2, "error allocating private path\n"), env);
		env.env[0] = ft_strjoin("PATH=", "/usr/local/bin:/usr/local/sbin:/usr/bin:/usr/sbin:/bin:/sbin:.:/.local/bin:/usr/local/vcpkg");
	} //change path so that it works at school ++ add protections in cd to check where we are to avoid seg faults ++ fix printenv and env to avoid seg faults
	env.env = dup_env(envp);
	return (env);
}

//pids
int    count_processes(t_jobs **jobs)//take double pointer
{
    int		i;
    t_jobs *job;

    i = 0;
    job = *jobs;
    while (job)
    {
        if (job->type == PIPE || job->job)
            i++;
        job = job->next;
    }
    return (i + 1);
}

void	*ft_calloc_pids(t_jobs *job)
{
	int		*dest;
	int		i;
	int		size;

	size = count_processes(&job);
	dest = malloc(sizeof(pid_t) * size);
	if (!dest)
		return (NULL);
	i = 0;
	while (i < size)
	{
		dest[i] = -1;
		i++;
	}
	return (dest);
}

