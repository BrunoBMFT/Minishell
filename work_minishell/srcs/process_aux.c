/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_aux.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 19:13:31 by bruno             #+#    #+#             */
/*   Updated: 2024/08/16 01:10:49 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


void	ft_perror_exit(char *str)//remove
{
	write(2, "minishell: ", 11);
	perror (str);
}
// ! RENAME
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
// ! RENAME
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
		execve(path, cmd, env);//error check
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
	//! DECLARING VARIABLES HERE? 
//	free_array(command);
	exit (0);//free fds
}

