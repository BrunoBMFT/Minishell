/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 19:13:31 by bruno             #+#    #+#             */
/*   Updated: 2024/08/14 18:53:06 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_perror_exit(char *str)//have this recieve exit code
{
	write(2, "minishell: ", 11);
	perror (str);
}

int find_command_path(char **cmd, char **env)
{
	char	**path_array;
	char	*path;
	char	*partial;//remove?
	int		i;

	path = ft_getenv("PATH", env);//error check
	path_array = ft_split(path, ':');//error check
	if (!path_array)
		exit (0);//exit code
	free (path);
	i = 0;
	while (path_array[i])
	{
		partial = ft_strjoin(path_array[i], "/");//error check
		path = ft_strjoin(partial, cmd[0]);
		free (partial);
		if (access(path, F_OK) == 0)
		{
			free_array(path_array);
			execve(path, cmd, env);
		}
		free (path);
		i++;
	}
	printf("%s: command not found\n", cmd[0]);
	free_array(path_array);
	exit (127);
}

char	*fix_cmd(char *cmd)
{
	char	*newcmd;

	if (cmd[0] == '~')
		newcmd = ft_strjoin(".", cmd + 1);//replaces the ~ for a . //error check
	else
		newcmd = cmd;
	return (newcmd);
}

int find_executable_path(char **cmd, char **env)//find better way to update command
{
	char	*path = NULL;
	char	cwd[PATH_MAX];
	char	*dir;
	
	//use absolute path from beginning

	if (*cmd[0] == '~')
	{
		dir = ft_getenv("HOME", env);//error check
		path = ft_strjoin(dir, "/");//error check
		path = ft_strjoin(path, *cmd + 2);//error check
	}
	else
	{
		dir = getcwd(cwd, PATH_MAX);//error check
		path = ft_strjoin(dir, "/");//error check
		path = ft_strjoin(path, *cmd);//error check
	}
	*cmd = fix_cmd(*cmd);//error check
	if (access(path, F_OK) == 0)
		execve(path, cmd, env);
	ft_perror_exit(cmd[0]);
	exit (127);
}

int	execute_job(char **command, char **env)
{
//	char	*path;
	int status = 0;

	if (!command[0])
		return (ft_printf("job error\n"), 126);//fix the error return, test with "" as input
	if (ft_strchr(command[0], '/'))
		status = find_executable_path(&command[0], env);
	else
		status = find_command_path(&command[0], env);
//	free_array(command);
	exit (0);//free fds
}

int	child_process(t_jobs *job, char **env, char **temp_vars)
{
	pid_t	pid;
	int		fd[2];
	int		status = 0;

	if (ft_strcmp(job->job[0], "cd") == 0)//not really working with multiple jobs
		return (caught_cd(job, env));
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

	if (ft_strcmp(job->job[0], "cd") == 0)//not really working with multiple jobs
		return (caught_cd(job, env));
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
