/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 17:26:33 by bruno             #+#    #+#             */
/*   Updated: 2024/11/01 02:43:36 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	executor_input(t_jobs *job, t_env *env)
{
	int	redirected_input;

	job->input = unquote_and_direct(job->input, env);
	if (ft_strcmp(job->input, "/dev/null") == 0)
		env->status = 1;
	redirected_input = open(job->input, O_RDONLY);
	dup2(redirected_input, STDIN_FILENO);
	close(redirected_input);
}

void	executor_output(t_jobs *job, t_env *env)
{
	int	redirected_output;

	job->output = unquote_and_direct(job->output, env);
	if (job->append)
		redirected_output = open(job->output,
				O_CREAT | O_APPEND | O_RDWR, 0644);
	else
	{
		if (access(job->output, F_OK) == 0)
			remove(job->output);
		redirected_output = open(job->output, O_CREAT | O_RDWR, 0644);
	}
	dup2(redirected_output, STDOUT_FILENO);
	close(redirected_output);
}

void	init_executor(t_jobs *job, t_env *env)
{
	env->saved_stdin = dup(STDIN_FILENO);
	env->saved_stdout = dup(STDOUT_FILENO);
	env->pids = ft_calloc_pids(job);
	if (!env->pids)
		return ;
	env->piped = false;
}

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
	init_executor(job, env);
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
