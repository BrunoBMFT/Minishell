/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_aux.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 19:13:31 by bruno             #+#    #+#             */
/*   Updated: 2024/09/04 22:22:14 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_perror_exit(char *str)//remove
{
	write(2, "minishell: ", 11);
	perror (str);
}

int execute_command(char **cmd, char **env)
{
	char	**path_array;
	char	*path;
	int		i;

	path = ft_getenv("PATH", env);//error check
	path_array = ft_split(path, ':');
	if (!path_array)
		exit (127);//free path_array
	free (path);
	i = 0;
	while (path_array[i])
	{
		path = ft_strjoin3(path_array[i], "/", cmd[0]);
		if (!path)
			return (free_array(path_array), 0);//free path
		if (access(path, F_OK) == 0)
		{
			free_array(path_array);
			execve(path, cmd, env);
		}
		free (path);
		i++;
	}
	ft_printf_fd(2, "minishell: %s: command not found\n", cmd[0]);
	free_array(path_array);
	exit (127);
}

char	**fix_cmd(char **cmd)
{
	char	**newcmd = ft_calloc(sizeof(char *), ft_split_wordcount(cmd) + 1);

	if (cmd[0][0] == '~')
	{
		newcmd[0] = ft_strjoin(".", *cmd + 1);//error check
		if (!newcmd[0])
			return (NULL);//cant return NULL
	}
	int i = 0;
	while (cmd[i])
	{
		newcmd[i] = ft_strdup(cmd[i]);
		if (!newcmd[i])
			return (NULL);//error check
		i++;
	}
	newcmd[i] = NULL;
	return (newcmd);
}

//call this function inside exec_exec so if no path is found it returns
char	*find_executable_path(char **cmd)
{
	char	*path;
	char	cwd[PATH_MAX];

/* 	if (**cmd == '~')//supposedly dont need to handle this
	{
		dir = ft_getenv("HOME", env);//error check
		path = ft_strjoin3(dir, "/", *cmd + 2);//error check
	} */
	if (*cmd[0] == '/')
		path = cmd[0];
	else
	{
		getcwd(cwd, PATH_MAX);//error check
		path = ft_strjoin3(cwd, "/", *cmd);//error check
	}
	if (access(path, F_OK) == 0)
		return (path);
	return (NULL);
}

int execute_executable(char **cmd, char **env)
{
	char	*path = NULL;

	path = find_executable_path(cmd);
	if (!path)
		return (ft_printf_fd(2, "minishell: %s: No such file or directory\n", cmd[0]));
	cmd = fix_cmd(cmd);
	if (cmd)
		ft_perror_exit(cmd[0]);//check if it's correct
	free_array(cmd);
	free (path);
	exit (127);
}

int	execute_job(char **command, char **env)
{
	int 	status = 0;
	if (!command[0])
		return (ft_printf("job error\n"), 126);//fix the error return, test with "" as input
	if (ft_strchr(command[0], '/'))
		status = execute_executable(command, env);
	else
		status = execute_command(command, env);
//	if it returns, everything needs to be freed
//	free fds in case of error
	exit (127);//check exit code
}

