/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brfernan <brfernan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 19:13:31 by bruno             #+#    #+#             */
/*   Updated: 2024/08/20 19:19:44 by brfernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	child_process(t_jobs *job, char **env, char ***temp_vars)
{
	pid_t	pid;
	int		fd[2];
	int		status = 0;

	if (ft_strcmp(job->job[0], "cd") == 0)
		return (caught_cd(job, env));
	pipe(fd);
	pid = new_fork();
	if (pid == 0)
	{
		close(fd[READ]);
		dup2(fd[WRITE], STDOUT_FILENO);//error check
		close(fd[WRITE]);
		if (try_builtins(job, env, temp_vars, true) == 200)
			execute_job(job->job, env);
	}
	close(fd[WRITE]);
	dup2(fd[READ], STDIN_FILENO);//error check
	close(fd[READ]);
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}

int	simple_process(t_jobs *job, char **env, char ***temp_vars)
{
	pid_t	pid;
	int	status;

	status = try_builtins(job, env, temp_vars, false);
	if (status != 200)
		return (WEXITSTATUS(status));
	pid = new_fork();
	if (pid == 0)
	{
/* 		bash builtins like echo recieve shell variables (temp_vars), we can check with echo
		execve and others wont be able to have those variables, we can check with printenv
		so maybe no temp_vars needs to be sent in, 
			as execve doesnt take it, unless we have to expand it before */
		execute_job(job->job, env);
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
