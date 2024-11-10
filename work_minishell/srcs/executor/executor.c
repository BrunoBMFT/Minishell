/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 17:26:33 by bruno             #+#    #+#             */
/*   Updated: 2024/11/09 18:14:06 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	job_reset(t_jobs *job, t_env *env)
{
	dup2(env->saved_stdin, STDIN_FILENO);
	dup2(env->saved_stdout, STDOUT_FILENO);
	if (job->heredoc_file && access(job->heredoc_file, F_OK) == 0)
		remove(job->heredoc_file);
	env->redir_error_flag = false;
}

void	executor_statements(t_jobs **job, t_env *env)
{
	if ((*job)->next && (*job)->next->type == AND)
	{
		env->piped = false;
		if (env->status == 0)
			*job = (*job)->next->next;
	}
	else if ((*job)->next && (*job)->next->type == OR)
	{
		env->piped = false;
		if (env->status == 0) // TODO WRONG AS FUCK
		{
			while ((*job)->next && (*job)->next->type == OR)
				*job = (*job)->next->next;
			if ((*job)->next)
				*job = (*job)->next->next;
			else
				*job = (*job)->next;
		}
		else
			*job = (*job)->next;
	}
	else
		*job = (*job)->next;
}

void	finish_executor(t_jobs *job, t_env *env)
{
	int	i;
	int	status;

	i = 0;
	if (access(".heredoc", F_OK) == 0)
		remove(".heredoc");
	while (env->pids[i] != -1)
	{
		waitpid(env->pids[i], &status, 0);
		env->pids[i] = -1;
		if (env->status == 0)
			env->status = WEXITSTATUS(status);
		i++;
	}
	close(env->saved_stdin);
	close(env->saved_stdout);
	free (env->pids);
}

void	start_pipe(t_jobs **job, t_env *env)
{
	env->piped = true;
	do_child_process((*job), env);
	*job = (*job)->next->next;
}

void	start_executor(t_jobs *job, t_env *env)
{
	if (!init_executor(job, env))
		return ;
	while (job)
	{
		if (job->job)
			job->job = modify_array(job->job, env);
		env->status = 0;//status reset, find a good place for this
		if (job->input)
			executor_input(job, env);
		if (job->output)
			executor_output(job, env);
		if (job->next && job->next->type == PIPE)
		{
			start_pipe(&job, env);
			continue ;
		}
		else if (job->job && env->piped)
			do_child_process(job, env);
		else if (job->job)
			do_simple_process(job, env);
		job_reset(job, env);
		executor_statements(&job, env);
	}
	finish_executor(job, env);
	return ;
}
