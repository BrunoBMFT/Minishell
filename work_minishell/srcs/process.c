/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 19:13:31 by bruno             #+#    #+#             */
/*   Updated: 2024/08/11 05:55:16 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*find_path(char *cmd, char **envp)
{
	char	**path_array;
	char	*path;
	char	*partial;
	int		i;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		return (NULL);
	path_array = ft_split(envp[i], ':');
	if (!path_array)
		return (NULL);//free stuff and error check
	i = 0;
	while (path_array[i])
	{
		partial = ft_strjoin(path_array[i], "/");
		path = ft_strjoin(partial, cmd);
		free (partial);
		if (access(path, F_OK) == 0)
			return (free_array(path_array), path);
		free (path);
		i++;
	}
	free_array(path_array);
	return (0);
}

int	run_executable(char **command, char **env)//handle ./ ../ ~/ and ./.././../ all possibilities
{
	//if command == ./ --> remove . and strjoin with getcwd
	//if command == ../ --> possibly getcwd and remove the last folder:
	//		/home/bruno/work_minishell/ --> /home/bruno/
	//if command == ~/ --> getenv HOME and strjoin the getenv
}

int	execute_job(char **command, char **env)
{
	char	*path;
	char cwd[100];

	if (!command[0])
		return (ft_printf("job error\n"), 126);//fix the error return, test with "" as input
	if (ft_strncmp(command[0], "./", 2) == 0)
	{
		printf("executable\n");
		char *temp1 = ft_strjoin(getcwd(cwd, sizeof(cwd)), command[0] + 1);//fix direct paths: cd includes & ../minishell
		execve(temp1, command, env);
	}//executable can run with something else in job?
	else
	{
		path = find_path(command[0], env);
		if (!path)
		{
			return (ft_printf("no command\n"), 127);
		/* 		free (path);
			exit (127);//free the commands and exit */
		}
		execve(path, command, env);
	}
	printf("execve failed\n");
	free_array(command);
	exit (0);//free fds
}

int	child_process(t_jobs *job, char **env, char **temp_vars)
{
	pid_t	pid;
	int		fd[2];
	int		status = 0;

/* 	if (ft_strcmp(job->cmd, "cd") == 0)//not really working with multiple jobs
		return (caught_cd(job, env)); */
	pipe(fd);
	pid = new_fork();
	if (pid == 0)
	{
		close(fd[READ]);
		dup2(fd[WRITE], STDOUT_FILENO);
		close(fd[WRITE]);
//		if (try_builtins(job, env, temp_vars) == 200)
			execute_job(job->job, env);
	}
	close(fd[WRITE]);
	dup2(fd[READ], STDIN_FILENO);
	close(fd[READ]);
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}

int	simple_process(t_jobs *job, char **env, char **temp_vars)
{
	pid_t	pid;
	int	status;

/* 	if (ft_strcmp(job->cmd, "cd") == 0)//not really working with multiple jobs
		return (caught_cd(job, env)); */
	status = 0;
	pid = new_fork();
	if (pid == 0)
	{
//		if (try_builtins(job, env, temp_vars) == 200)
			execute_job(job->job, env);//has to take in temp_vars as well, stuff like unset?
	}
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
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
