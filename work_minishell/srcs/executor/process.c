/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 19:13:31 by bruno             #+#    #+#             */
/*   Updated: 2024/11/01 02:47:59 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	new_fork(t_env *env)
{
	pid_t	pid;
	int		i;

	i = 0;
	while (env->pids[i] != -1)
		i++;
	env->pids[i] = fork();
	if (env->pids[i] < 0)
	{
		ft_printf_fd(2, "fork() error\n");
		env->status = 1;
		return (-1);
	}
	return (env->pids[i]);
}

void	child(t_jobs *job, t_env *env, int fd[])
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
	clean_exit(job, env, env->status);
}

void	do_child_process(t_jobs *job, t_env *env)// ! signals
{
	int		fd[2];
	pid_t	pid;

	choose_sig(IGNORE_SIG);//correct?
	pipe(fd);
	pid = new_fork(env);
	if (pid < 0)
		return ;
	if (pid == 0)
		child(job, env, fd);
	close(fd[WRITE]);
	dup2(fd[READ], STDIN_FILENO);
	close(fd[READ]);
	return ;
}

void	do_simple_process(t_jobs *job, t_env *env)
{
	int		status;
	pid_t	pid;

	choose_sig(IGNORE_SIG);
	if (job->job[0] && ft_strcmp(job->job[0], "cd") == 0)
		return (caught_cd(job, env));
	status = try_builtins(job, env);
	if (status != 200)
		return (env->status = status, (void) NULL);
	pid = new_fork(env);
	if (pid < 0)
		return ;
	if (pid == 0)
		execute_job(job, env);
	return ;
}
