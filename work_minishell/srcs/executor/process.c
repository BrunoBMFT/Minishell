/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 19:13:31 by bruno             #+#    #+#             */
/*   Updated: 2024/10/27 12:11:50 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	new_fork()
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		ft_printf_fd(2, "fork() error\n");
		return (-1);
	}
	return (pid);
}

void	child_process(t_jobs *job, t_env *env)// ! signals
{
	int		fd[2];

	pipe(fd);
	int i = 0;
	while (env->pids[i] != -1)
		i++;
	env->pids[i] = new_fork();
	if (env->pids[i] < 0)
		return (env->status = 1, (void)NULL);//error check
	if (env->pids[i] == 0)
	{
		close(fd[READ]);
		if (!job->output && job->next && job->next->type == PIPE)
			dup2(fd[WRITE], STDOUT_FILENO);
		else if (!job->output)
			dup2(env->saved_stdout, STDOUT_FILENO);
		close(fd[WRITE]);
		env->status = try_builtins(job, env);
		if (env->status == 200)
			execute_job(job, env);
		clean_exit(job, env, env->status);//env.status
	}
	close(fd[WRITE]);
	dup2(fd[READ], STDIN_FILENO);
	close(fd[READ]);
	return ;
}

void	simple_process(t_jobs *job, t_env *env)
{
	int		status;

	if (!job->job[0])
		return ;
	choose_signal(IGNORE_SIG);
	if (job->job && job->job[0] && (ft_strcmp(job->job[0], "cd")) == 0)//a few of this checks are done before
		return (caught_cd(job, env));//fix exit status and return value?
	status = try_builtins(job, env);
	if (status != 200)
		return (env->status = status, (void)NULL);
	int i = 0;
	while (env->pids[i] != -1)
		i++;
	env->pids[i] = new_fork();
	if (env->pids[i] < 0)
		return (env->status = 1, (void)NULL);//error check
	if (env->pids[i] == 0)
		execute_job(job, env);//error check?
	return ;
}
