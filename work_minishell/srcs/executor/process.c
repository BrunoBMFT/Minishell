/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 19:13:31 by bruno             #+#    #+#             */
/*   Updated: 2024/10/24 15:59:42 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	child_process(t_jobs *job, t_env *env)// ! signals
{
//	pid_t	pid;
	int		fd[2];
	int		status = 0;

	pipe(fd);//error check?
	int i = 0;//bad
	while (env->pids[i] == -1)//bad
		i++;//bad
	env->pids[i] = fork();//error check
	if (env->pids[i] < 0)
		return(ft_printf_fd(2, "fork() error\n"), 1);
	if (env->pids[i] == 0)
	{
		close(fd[READ]);
		if (!job->output && job->next && job->next->type == PIPE)//checks whether it is last statement or not
			dup2(fd[WRITE], STDOUT_FILENO);//error check
		else if (!job->output)
			dup2(env->saved_stdout, STDOUT_FILENO);//error check
		close(fd[WRITE]);
		if (try_builtins(job, env, true) == 200)
			execute_job(job, env);
		clean_exit(job, env, status);
	}
	close(fd[WRITE]);
	dup2(fd[READ], STDIN_FILENO);//error check
	close(fd[READ]);
	
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
		
	pid = fork();// ! change this pid as well?
	if (pid < 0)
		return(ft_printf_fd(2, "fork() error\n"), 1);
	
	if (pid == 0)
		execute_job(job, env);//error check?
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}
