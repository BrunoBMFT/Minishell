/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 19:13:31 by bruno             #+#    #+#             */
/*   Updated: 2024/08/13 03:48:26 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


//executable can run with something else in job
//		./a.out hello world (int ac, char **av)
/* 
char	*remove_last_word(char *str)
{
	char	*new = NULL;
	char	*temp;
	char	**split;
	int	i;

	split = ft_split(str, '/');//error check
	i = 0;
	while (split[i] && split[i + 1])
	{
		if (!new)//error check
			temp = ft_strdup("/");//error check
		else
			temp = ft_strjoin(new, "/");//error check
		if (new)
			free (new);
		new = ft_strjoin(temp, split[i]);//error check
		free (temp);
		i++;
	}
	free_array(split);
	return (new);
}

char	*fix_command(char *str)
{
	char	*command = NULL;
	char	**split;
	int		i;

	i = 0;
	split = ft_split(str, '/');//error check
	while (split[i] && split[i + 1])
		i++;
	command = ft_strjoin("./", split[i]);//error check
	return (command);
}

// ! FIX COMMAND
char	*find_executable_path(char *command, char **env)//handle ./ ../ ~/ and ./.././../ all possibilities
{
	char	*path = NULL;
	char	cwd[100];//size fix
	
	//use absolute path from beginning

	
	if (ft_strncmp(command, "./", 2) == 0)
		path = ft_strjoin(getcwd(cwd, sizeof(cwd)), command + 1);//skips the .  //error check
	else if (ft_strncmp(command, "../", 3) == 0)
		path = ft_strjoin(remove_last_word(getcwd(cwd, sizeof(cwd))), command + 2);//skips the ..  //error check
	else if (ft_strncmp(command, "~/", 2) == 0)
		path = ft_strjoin(ft_getenv("HOME", env), command + 1);



	command = fix_command(path);//error check
	printf("path: %s\ncommand: %s\n", path, command);
	return (path);

	
	if (ft_strncmp(command, "./", 2) == 0)
		path = ft_strjoin(getcwd(cwd, sizeof(cwd)), command + 1);//skips the .  //error check
	else if (ft_strncmp(command, "../", 3) == 0)
		path = ft_strjoin(remove_last_word(getcwd(cwd, sizeof(cwd))), command + 2);//skips the ..  //error check
	else if (ft_strncmp(command, "~/", 2) == 0)
		path = ft_strjoin(ft_getenv("HOME", env), command + 1);
} */

char	*find_command_path(char *cmd, char **envp)
{
	char	**path_array;
	char	*path;
	char	*partial;//remove?
	int		i;

	path_array = ft_split(ft_getenv("PATH", envp), ':');
	if (!path_array)
		return (NULL);//free stuff and error check
	i = 0;
	while (path_array[i])
	{
		partial = ft_strjoin(path_array[i], "/");//error check
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

char	*fix_command(char *command)
{
	char	*newcommand;

	if (command[0] == '~')
		newcommand = ft_strjoin(".", command + 1);//replaces the ~ for a . //error check
	else
		newcommand = command;
	return (newcommand);
}
//this function shouldnt run
char	*find_executable_path(char *command, char **env)//handle ./ ../ ~/ and ./.././../ & work/minishell has to work
{
	char	*path = NULL;
	char	cwd[100];//size fix
	
	//use absolute path from beginning

	char *dir;
	if (command[0] == '~')
	{
		dir = ft_getenv("HOME", env);//error check
		path = ft_strjoin(dir, "/");//error check
		path = ft_strjoin(path, command + 2);//error check
	}
	else
	{
		dir = getcwd(cwd, sizeof(cwd));//error check
		path = ft_strjoin(dir, "/");//error check
		path = ft_strjoin(path, command);//error check
	}
	if (access(path, F_OK) == 0)
		return (path);
	return (NULL);
}

int	execute_job(char **command, char **env)
{
	char	*path;

	if (!command[0])
		return (ft_printf("job error\n"), 126);//fix the error return, test with "" as input
	path = find_command_path(command[0], env);
	if (!path)
	{
		path = find_executable_path(command[0], env);
		command[0] = fix_command(command[0]);//error check
		if (!path)
		{
			ft_printf("no command\n");
			free (path);
			exit (127);//free the commands and exit
		}
	}
 	int status = execve(path, command, env);
	printf("execve failed %d\n", status);//error message should be here
	free_array(command);
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
		if (try_builtins(job, env, temp_vars) == 200)
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
		if (try_builtins(job, env, temp_vars) == 200)
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
