/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_aux.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 18:25:38 by bruno             #+#    #+#             */
/*   Updated: 2024/11/14 17:47:27 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

void	finish_executor(t_jobs *job, t_env *env)
{
	if (access(".heredoc", F_OK) == 0)
		remove(".heredoc");
	run_waitpids(env);
	close(env->saved_stdin);
	close(env->saved_stdout);
	free (env->pids);
	env->redir_error = false;
}

void	job_reset(t_jobs *job, t_env *env)
{
	dup2(env->saved_stdin, STDIN_FILENO);
	dup2(env->saved_stdout, STDOUT_FILENO);
	if (job->heredoc_file && access(job->heredoc_file, F_OK) == 0)
		remove(job->heredoc_file);
}

bool	executor_statements(t_jobs **job, t_env *env)
{
	job_reset(*job, env);
	if ((*job)->next && (*job)->next->type == AND)
	{
		env->piped = false;
		run_waitpids(env);
		if (env->status == 0)
			*job = (*job)->next->next;
		else
			return (false);
	}
	else if ((*job)->next && (*job)->next->type == OR)
	{
		env->piped = false;
		run_waitpids(env);
		if (env->status == 0)
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
	return (true);
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
