/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 19:13:31 by bruno             #+#    #+#             */
/*   Updated: 2024/07/23 16:54:28 by bruno            ###   ########.fr       */
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
		if (access(path, F_OK) == 0)
			return (path);
		free (path);
		paths++;
	}
	free_array(paths);
	return (NULL);
}

bool	execute_job(char **command, char **env)
{
	char	*path = find_path(command[0], env);
	if (!path)
		return (printf("no command\n"), false);//free the commands
	execve(path, command, env);
	printf("execve failed\n");
	free_array(command);
	return true;
}

int	child_process(t_jobs *job, char **env)
{
	int		fd[2];
	int		status = 0;

	pipe(fd);
	pid_t pid = new_fork();
	if (pid == 0)
	{
/* 		sig = 1;
		if (set_up_signal(handle_sigint) < 0);
			clean_exit(job, NULL, NULL); */
		close(fd[READ]);
		dup2(fd[WRITE], STDOUT_FILENO);
		close(fd[WRITE]);
		if (!execute_job(job->job, env))
			panic("execute");
	}
	close(fd[WRITE]);
	dup2(fd[READ], STDIN_FILENO);
	close(fd[READ]);
	waitpid(pid, &status, 0);
	return WEXITSTATUS(status);
}

int	simple_process(t_jobs *job, char **env)
{
	int	pid = new_fork();
	if (pid == 0)
	{
/* 		sig = 1;
		if (set_up_signal(handle_sigint) < 0);
			clean_exit(job, NULL, NULL); */
		if (!execute_job(job->job, env))
			panic("simple execute failed\n");//free fds, show exit code and perror
	}
	int status = 0;
	waitpid(pid, &status, 0);
//	printf("status: %d\n", WEXITSTATUS(status));
	return (WEXITSTATUS(status));
}
