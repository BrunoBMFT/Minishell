/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 19:13:31 by bruno             #+#    #+#             */
/*   Updated: 2024/08/09 04:27:11 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*find_path(char *command, char **env)
{
	char	**paths;
	char	*path;
	char	*temp;
	int		i;

	i = 0;
	while (*env && ft_strnstr(*env, "PATH", 4) == 0)
		env++;
	if (!*env)
		return NULL;
	paths = ft_split(*env, ':');//error check
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		path = ft_strjoin(temp, command);
		free (temp);
		if (access(path, F_OK) == 0)
		{
			free_array(paths);
			return (path);
		}
		free (path);
		i++;
	}
	free_array(paths);
	printf("minishell: command not found: %s\n", command);
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
