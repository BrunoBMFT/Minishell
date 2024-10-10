/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_aux.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 19:13:31 by bruno             #+#    #+#             */
/*   Updated: 2024/10/07 21:42:17 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*find_command_path(char	**cmd, t_env *env)
{
	char	**path_array;
	char	*path;
	int		i;

	path = ft_getenv("PATH", env->env);
	path_array = ft_split(path, ':');
	free (path);
	if (!path_array)
		return (NULL);
	i = 0;
	while (path_array[i])
	{
		path = ft_strjoin3(path_array[i], "/", cmd[0]);
		if (!path)
			return (free_array(path_array), NULL);
		if (access(path, F_OK) == 0)
			return (free_array(path_array), path);
		free (path);
		i++;
	}
	free_array(path_array);
	return (ft_printf_fd(2, "minishell: %s: command not found\n", cmd[0]), NULL);
}

void	execute_command(t_jobs *job, t_env *env)
{
	char	*path;

	path = find_command_path(job->job, env);
	if (!path)
		clean_exit(job, env, 127);
	execve(path, job->job, env->env);
	clean_exit(job, env, 127);
}

char	*find_executable_path(char *cmd)
{
	char	*path;
	char	cwd[PATH_MAX];

	if (cmd[0] == '/')
		path = cmd;
	else
	{
		getcwd(cwd, PATH_MAX);//error check
		path = ft_strjoin3(cwd, "/", cmd);//error check
		if (!path)
			return (NULL);
	}
	if (access(path, F_OK) == 0)
		return (path);
	free (path);
	ft_printf_fd(2, "minishell: %s: No such file or directory\n", cmd);
	return (NULL);
}

void execute_executable(t_jobs *job, t_env *env)
{
	char	*path;

	path = find_executable_path(job->job[0]);
	if (!path)
		clean_exit(job, env, 127);
	execve(path, job->job, env->env);
	ft_printf_fd(2, "execve() failed\n");
	free (path);
	clean_exit(job, env, 127);
}

int	execute_job(t_jobs *job, t_env *env)
{
	if (!job->job[0])//check execd being ""
		return (ft_printf("job error\n"), 126);
//	choose_signal(CHILD_SIG);
//	choose_signal(IGNORE_SIG);
	if (ft_strchr(job->job[0], '/'))
		execute_executable(job, env);
	else
		execute_command(job, env);
	return (0);
}

