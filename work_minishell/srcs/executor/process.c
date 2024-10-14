/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 19:13:31 by bruno             #+#    #+#             */
/*   Updated: 2024/10/13 21:04:53 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	child_process(t_jobs *job, t_env *env)
{
	pid_t	pid;
	int		fd[2];
	int		status = 0;

	pipe(fd);//error check?
	pid = fork();
	if (pid < 0)
		return(ft_printf_fd(2, "fork() error\n"), 1);
	if (pid == 0)
	{
		close(fd[READ]);
		if (job->next && job->next->type == PIPE)
			dup2(fd[WRITE], STDOUT_FILENO);//error check
		close(fd[WRITE]);
		if (try_builtins(job, env, true) == 200)
			execute_job(job, env);
		clean_exit(job, env, status);
	}
	close(fd[WRITE]);
	dup2(fd[READ], STDIN_FILENO);//error check
	close(fd[READ]);
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}

int	simple_process(t_jobs *job, t_env *env)
{
	pid_t	pid;
	int	status;

	choose_signal(IGNORE_SIG);

	if (job->job && job->job[0] && (ft_strcmp(job->job[0], "cd")) == 0)
		return (caught_cd(job, env));

	status = try_builtins(job, env, false);
	if (status != 200)
		return (status);
		
	pid = fork();
	if (pid < 0)
		return(ft_printf_fd(2, "fork() error\n"), 1);
	
	if (pid == 0)
		execute_job(job, env);//error check?
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}
