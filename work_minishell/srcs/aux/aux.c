/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 19:15:54 by bruno             #+#    #+#             */
/*   Updated: 2024/11/10 00:45:50 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_getpid(void)
{
	int		fd;
	char	buffer[256];
	int		pid;

	fd = open("/proc/self/stat", O_RDONLY);
	if (fd < 0)
		return (ft_putendl_fd("minishell: getpid() error", 2), 0);
	read(fd, buffer, 255);
	close (fd);
	pid = ft_atoi(buffer);
	return (pid);
}

char	*update_prompt(void)
{
	char	cwd[PATH_MAX];
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
		return (free_array(folders), ft_strdup("/$ "));
	while (folders[i])
		i++;
	prompt = ft_strjoin(folders[i - 1], "$ ");
	free_array(folders);
	if (!prompt)
		return (NULL);
	return (prompt);
}

char	**dup_env(char **envp)
{
	char	**new_env;
	char	*temp;
	int		i;

	i = 0;
	new_env = ft_calloc(sizeof(char *), ft_split_wordcount(envp) + 2);
	if (!new_env || !envp || !envp[0])
		return (NULL);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "SHLVL=", 6) == 0)
		{
			temp = ft_itoa(ft_atoi(envp[i] + 6) + 1);
			new_env[i] = ft_strjoin("SHLVL=", temp);
			free (temp);
		}
		else
			new_env[i] = ft_strdup(envp[i]);
		if (!new_env[i])
			free_array(new_env);//check
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

t_env	init_env(char **envp)
{
	t_env	env;
	char	buf[PATH_MAX];

	env.prompt = NULL;
	env.env = NULL;
	env.status = 0;
	env.redir_error_flag = false;
	if (!envp || !envp[0])
	{
		env.env = malloc(sizeof (char *) * 3);
		if (!env.env)
			return (ft_printf_fd(2, "error allocating private path\n"), env);
		env.env[0] = ft_strdup("PATH=/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin:/snap/bin:");
		env.env[1] = ft_strjoin("PWD=", getcwd(buf, PATH_MAX));
		env.env[2] = NULL;
	}
	else
		env.env = dup_env(envp);
	return (env);
}

int	count_processes(t_jobs **jobs)
{
	int		i;
	t_jobs	*job;

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
