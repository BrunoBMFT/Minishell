/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 17:26:33 by bruno             #+#    #+#             */
/*   Updated: 2024/11/24 03:05:31 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	executor(t_jobs *job, t_env *env)
{
	if (!init_executor(job, env))
		return ;
	while (job)
	{
		if (job->job)
			modify_array(job->job, env);
		if (!executor_input(job, env) || !executor_output(job, env))
		{
			job = job->next;
			continue ;
		}
		if (job->next && job->next->type == PIPE)
		{
			start_pipe(&job, env);
			continue ;
		}
		else if (job->job && env->piped)
			piped_process(job, env);
		else if (job->job)
			simple_process(job, env);
		if (!executor_statements(&job, env))
			break ;
	}
	finish_executor(job, env);
	return ;
}

void	start_pipe(t_jobs **job, t_env *env)
{
	t_jobs *temp1;
	t_jobs *temp2;

	temp1 = (*job);
	temp2 = (*job)->next;
	env->piped = true;
	piped_process((*job), env);
	*job = (*job)->next->next;
	clear_single_job(&temp1);
	clear_single_job(&temp2);
}

bool	executor_input(t_jobs *job, t_env *env)
{
	char	*temp;
	int		redirected_input;

	if (!job->input)
		return (true);
	temp = unquote_and_direct(job->input, env);
	if (job->input)
		free (job->input);
	job->input = temp;
	if (ft_strcmp(job->input, "/dev/null") == 0)
		return (env->status = 1, false);
	redirected_input = open(job->input, O_RDONLY);
	dup2(redirected_input, STDIN_FILENO);
	close(redirected_input);
	return (true);
}

bool	executor_output(t_jobs *job, t_env *env)
{
	char	*temp;
	int		redirected_output;

	if (!job->output)
		return (true);
	temp = unquote_and_direct(job->output, env);
	if (job->output)
		free (job->output);
	job->output = temp;
	if (ft_strcmp(job->output, "/dev/null") == 0)
		return (env->status = 1, false);
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
	return (true);
}

