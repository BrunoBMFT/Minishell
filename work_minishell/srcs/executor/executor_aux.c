/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_aux.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 18:25:38 by bruno             #+#    #+#             */
/*   Updated: 2024/12/02 16:00:14 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	job_reset(t_jobs *job, t_env *env)
{
	dup2(env->saved_stdin, STDIN_FILENO);
	dup2(env->saved_stdout, STDOUT_FILENO);
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

bool	run_and(t_jobs **job, t_env *env)
{
	env->piped = false;
	if (env->status == 0)
		skip_job_2(job);
	else
		return (clear_jobs(job), false);
	return (true);
}

void	run_or(t_jobs **job, t_env *env)
{

	env->piped = false;
	if (env->status == 0)
	{
		while ((*job)->next && (*job)->next->type == OR)
			*job = (*job)->next->next;
		if ((*job)->next && (*job)->next->type == OR)
			skip_job_2(job);
		else
			skip_job(job);
	}
	else
		*job = (*job)->next;
}

bool	loop_executor(t_jobs **job, t_env *env)
{
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
		skip_job(job);
	return (true);
}
