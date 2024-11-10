/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_aux.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 18:25:38 by bruno             #+#    #+#             */
/*   Updated: 2024/11/09 18:13:10 by bruno            ###   ########.fr       */
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

bool	init_executor(t_jobs *job, t_env *env)
{
	if (!job)
		return (false);
	env->saved_stdin = dup(STDIN_FILENO);
	env->saved_stdout = dup(STDOUT_FILENO);
	env->pids = ft_calloc_pids(job);
	if (!env->pids)
		return (false);
	env->piped = false;
	return (true);
}
