/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 19:13:31 by bruno             #+#    #+#             */
/*   Updated: 2024/09/09 22:21:17 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
//maybe if pipe is detected at any point in executor, executor bool pipe turns true
//	and after that, if pipe is true, always call child_process
//	like that, simple_process can be the only one with exit in it
//	but simple_process sends to builtins whether its piped or not, have to check
int	child_process(t_jobs *job, t_env env)
{
	pid_t	pid;
	int		fd[2];
	int		status = 0;

	pipe(fd);
	pid = new_fork();
	if (pid == 0)
	{
		close(fd[READ]);
		if (job->next && job->next->type == PIPE)//might not need this check
			dup2(fd[WRITE], STDOUT_FILENO);//error check
		close(fd[WRITE]);
		if (try_builtins(job, env, true) == 200)
			execute_job(job, env);
	}
	close(fd[WRITE]);
	dup2(fd[READ], STDIN_FILENO);//error check
	close(fd[READ]);
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}

int	simple_process(t_jobs *job, t_env env)//recieve pipe to decide exit?
{
	pid_t	pid;
	int	status;

	if (job->job && job->job[0] && (ft_strcmp(job->job[0], "cd")) == 0)
		return (caught_cd(job, env));
	status = try_builtins(job, env, false);//check if pipe needs to be sent
	if (status != 200)
		return (status);
	pid = new_fork();
	if (pid == 0)
	{
		execute_job(job, env);
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
		panic("fork() error");
	return (pid);
}
