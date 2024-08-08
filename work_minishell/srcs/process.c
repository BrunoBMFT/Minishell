/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 19:13:31 by bruno             #+#    #+#             */
/*   Updated: 2024/08/07 02:54:32 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*find_path(char *command, char **env)
{
	//use strcmp
	char	**paths;
	char	*path;

	while (*env && ft_strnstr(*env, "PATH", 4) == 0)
		env++;
	if (!*env)
		return NULL;
	paths = ft_split(*env, ':');
	while (*paths)
	{
		*paths = ft_strjoin(*paths, "/");
		path = ft_strjoin(*paths, command);
		free (*paths);
		if (access(path, F_OK) == 0)
			return (path);
		free (path);
		paths++;
	}
	free_array(paths);
	return (NULL);
}

int	execute_job(char **command, char **env)
{
	char	*path = find_path(command[0], env);
	if (!path)
	{
		free (path);
		exit (127);//free the commands and exit
	}
	execve(path, command, env);
	printf("execve failed\n");
	free_array(command);
	exit (0);//free fds
}

int	child_process(t_jobs *job, char **env, char **temp_vars)
{
	int		fd[2];
	int		status = 0;

	if (ft_strcmp(job->cmd, "cd") == 0)//not really working with multiple jobs
		return (caught_cd(job, env));
	pipe(fd);
	pid_t pid = new_fork();
	if (pid == 0)
	{
		close(fd[READ]);
		dup2(fd[WRITE], STDOUT_FILENO);
		close(fd[WRITE]);
		if (try_builtins(job, env, temp_vars) == 200)
			execute_job(job->job, env);
	}
	close(fd[WRITE]);
	dup2(fd[READ], STDIN_FILENO);
	close(fd[READ]);
	waitpid(pid, &status, 0);
	return WEXITSTATUS(status);
}

int	simple_process(t_jobs *job, char **env, char **temp_vars)
{
	int	pid;
	int	status;

	if (ft_strcmp(job->cmd, "cd") == 0)//not really working with multiple jobs
		return (caught_cd(job, env));
	status = 0;
	pid = new_fork();
	if (pid == 0)
	{
		if (try_builtins(job, env, temp_vars) == 200)
			execute_job(job->job, env);//has to take in temp_vars as well?
	}
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}
