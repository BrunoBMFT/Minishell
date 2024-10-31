/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 17:26:33 by bruno             #+#    #+#             */
/*   Updated: 2024/10/30 15:07:15 by bruno            ###   ########.fr       */
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
		redirected_output = open(job->output, O_CREAT | O_APPEND | O_RDWR, 0644);
	else
	{
		if (access(job->output, F_OK) == 0)
			remove(job->output);
		redirected_output = open(job->output, O_CREAT | O_RDWR, 0644);
	}
	dup2(redirected_output, STDOUT_FILENO);
	close(redirected_output);//else
}

void	start_executor(t_jobs *job, t_env *env)
{
	env->saved_stdin = dup(STDIN_FILENO);
	env->saved_stdout = dup(STDOUT_FILENO);
	env->pids = ft_calloc_pids(job);//error check
	env->piped = false;
	while (job)
	{
		//expanding
		if (job->job)
			job->job = modify_array(job->job, env);
		env->status = 0;
		//redirections
		if (job->input)
			executor_input(job, env);
		if (job->output)
			executor_output(job, env);
		
		//executing jobs
		if (job->next && job->next->type == PIPE)
		{
			env->piped = true;
			child_process(job, env);
			job = job->next->next;
			continue;
		}
		else if (job->job && env->piped)
			child_process(job, env);//builtins status check
		else if (job->job)
			simple_process(job, env);//builtins status check

		//resets
		dup2(env->saved_stdin, STDIN_FILENO);
		dup2(env->saved_stdout, STDOUT_FILENO);
		if (job->heredoc_file && access(job->heredoc_file, F_OK) == 0)
			remove(job->heredoc_file);
		//operators
		if (job->next && job->next->type == AND)
		{
			if (env->status == 0)
				job = job->next->next;
			env->piped  = false;
		}
		else if (job->next && job->next->type == OR)
		{
			if (env->status == 0)//WRONG AS FUCK
			{
				while(job->next && job->next->type == OR)
					job = job->next->next;
				if (job->next)
					job = job->next->next;
				else
					job = job->next;
			}
			else
				job = job->next;
			env->piped = false;
		}
		else
			job = job->next;
		env->redir_error_flag = false;
	}
	if (access(".heredoc", F_OK) == 0)
		remove(".heredoc");
	// TODO function for this
	int i = 0;
	while (env->pids[i] != -1)
	{
		int status;
		waitpid(env->pids[i], &status, 0);
		env->pids[i] = -1;
		if (env->status == 0)
			env->status = WEXITSTATUS(status);
		i++;
	}
	// TODO function for this
	close(env->saved_stdin);
	close(env->saved_stdout);
	free (env->pids);
	return ;
}
