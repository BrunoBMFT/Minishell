/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_aux.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brfernan <brfernan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 19:13:31 by bruno             #+#    #+#             */
/*   Updated: 2024/08/22 19:31:34 by brfernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_perror_exit(char *str)//remove
{
	write(2, "minishell: ", 11);
	perror (str);
}

int execute_command_path(char **cmd, char **env)
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
	printf("%s: command not found\n", cmd[0]);
	free_array(path_array);
	exit (127);
}

char	**fix_cmd(char **cmd)
{
	char	**newcmd = malloc(sizeof(char *) * 3);

	if (cmd[0][0] == '~')
		newcmd[0] = ft_strjoin(".", *cmd + 1);//replaces the ~ for a .
	else
		newcmd[0] = ft_strdup(*cmd);
	newcmd[1] = NULL;
//	if (!newcmd)//error check
//		return (NULL);//free newcmd
	return (newcmd);
}

int execute_executable_path(char **cmd, char **env)//find better way to update command
{
	char	*path = NULL;
	char	cwd[PATH_MAX];
	char	*dir;
	
	if (*cmd[0] == '~')
	{
		dir = ft_getenv("HOME", env);//error check
		path = ft_strjoin3(dir, "/", *cmd + 2);//error check
	}
	//fix absolute paths -> "~/", "/", ".", ".."
	else if (*cmd[0] == '/')
	{
		dir = *cmd;//dir???????????????????????????????????????????????
		path = cmd[0];
	}
	else
	{
		getcwd(cwd, PATH_MAX);//error check
		path = ft_strjoin3(cwd, "/", *cmd);//error check
		if (!path)
			return (0);//free path
	}
	cmd = fix_cmd(cmd);
//	if (!cmd)
//		return (0);//free cmd?
	printf("cmd: %s\npath: %s\n", *cmd, path);
	if (access(path, F_OK) == 0)
		execve(path, cmd, env);//error check?
	ft_perror_exit(cmd[0]);//check if it's correct
	exit (127);
}

int	execute_job(char **command, char **env)
{
	int status = 0;

	if (!command[0])
		return (ft_printf("job error\n"), 126);//fix the error return, test with "" as input
	if (ft_strchr(command[0], '/'))
		status = execute_executable_path(&command[0], env);
	else
		status = execute_command_path(&command[0], env);
//	if it returns, everything needs to be freed
//	free fds in case of error
	exit (127);//check exit code
}

