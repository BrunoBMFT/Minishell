/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_aux.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 18:25:38 by bruno             #+#    #+#             */
/*   Updated: 2024/11/24 18:34:10 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	job_reset(t_jobs *job, t_env *env)
{
	dup2(env->saved_stdin, STDIN_FILENO);//fd leaking
	dup2(env->saved_stdout, STDOUT_FILENO);//fd leaking
	if (job->heredoc_file && access(job->heredoc_file, F_OK) == 0)
		remove(job->heredoc_file);
}

void	run_waitpids(t_env *env)
{
	int	i;
	int	status;

	i = 0;
	while (env->pids[i] != -1)
	{
		waitpid(env->pids[i], &status, 0);
		env->pids[i] = -1;
		env->status = WEXITSTATUS(status);
		i++;
	}
}
//make a func that does the job.next.next while clearing the 2 jobs that are skipped
bool	run_and(t_jobs **job, t_env *env)
{
	t_jobs *temp1;//make func
	t_jobs *temp2;//make func

	temp1 = *job;//make func
	temp2 = (*job)->next;//make func
	env->piped = false;
	if (env->status == 0)
	{
		*job = (*job)->next->next;//make func
		clear_single_job(&temp1);//make func
		clear_single_job(&temp2);//make func
	}
	else
		return (clear_jobs(job), false);//leaks
	return (true);
}

void	run_or(t_jobs **job, t_env *env)
{
	t_jobs *temp1;
	t_jobs *temp2;

	env->piped = false;
	if (env->status == 0)
	{
		temp1 = (*job);
		temp2 = (*job)->next;
		while ((*job)->next && (*job)->next->type == OR)
		{
			*job = (*job)->next->next;
		}
		if ((*job)->next && (*job)->next->type == OR)
		{
			*job = (*job)->next->next;
			// clear_single_job(&temp1);//make func
			// clear_single_job(&temp2);//make func
		}
		else
		{
			// *job = (*job)->next;
		}
	}
	else
		*job = (*job)->next;
}

bool	loop_executor(t_jobs **job, t_env *env)
{
	t_jobs *temp;

	temp = *job;
	run_waitpids(env);
	job_reset(*job, env);
	if ((*job)->next && (*job)->next->type == AND)
	{
		if (!run_and(job, env))
			return (false);
	}
	else if ((*job)->next && (*job)->next->type == OR)
		run_or(job, env);
	else
	{
		*job = (*job)->next;
		clear_single_job(&temp);
	}
	return (true);
}
