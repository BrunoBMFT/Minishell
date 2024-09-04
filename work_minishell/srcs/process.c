/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 19:13:31 by bruno             #+#    #+#             */
/*   Updated: 2024/09/04 02:24:40 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	child_process(t_jobs *job, t_env env)
{
	pid_t	pid;
	int		fd[2];
	int		status = 0;

	if (job->job && job->job[0] && (ft_strcmp(job->job[0], "cd")) == 0)
		return (caught_cd(job, env.env));
	pipe(fd);
	pid = new_fork();
	if (pid == 0)
	{
		close(fd[READ]);
		if (job->next && job->next->type == PIPE)//might not need this check
			dup2(fd[WRITE], STDOUT_FILENO);//error check
		close(fd[WRITE]);
		if (try_builtins(job, env, true) == 200)
			execute_job(job->job, env.env);
	}
	close(fd[WRITE]);
	dup2(fd[READ], STDIN_FILENO);//error check
	close(fd[READ]);
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}

int	simple_process(t_jobs *job, t_env env)
{
	pid_t	pid;
	int	status;
	
	if (ft_strcmp(job->job[0], "exit") == 0)//exits even if piped
		status = caught_exit(job, env);
	status = try_builtins(job, env, false);
	if (status != 200)
		return (status);
	//has to test if executable_path exists here
	pid = new_fork();
	if (pid == 0)
	{
		execute_job(job->job, env.env);
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
		panic("fork");// TODO stop using panic
	return (pid);
}
