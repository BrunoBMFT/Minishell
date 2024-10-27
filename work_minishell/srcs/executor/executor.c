/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 17:26:33 by bruno             #+#    #+#             */
/*   Updated: 2024/10/27 12:19:20 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	executor_input(t_jobs *job, t_env *env)
{
	int	redirected_input;
	
/* 
	if (job->input && (job->input[0] == '$'))// TODO for now commented, messes up apply_redir
	{
//		*status = 1;
//		return (ft_printf_fd(2, "minishell: %s: ambiguous redirect\n", job->input), -1);
	} */
	job->input = unquote_and_direct(job->input, env);
	//the reason why input was in lexer was so that many inputs can be processed
	if (access(job->input, F_OK) != 0)
	{
		if (!job->redir_error_flag)//FUCKING STUPID ITS NOT WORKING
		{
			ft_printf_fd(2, "bash: %s: No such file or directory\n", job->input);
			job->redir_error_flag = true;//FUCKING STUPID ITS NOT WORKING
		}
		job->input = ft_strdup("/dev/null");
		env->status = 1;
	}
	redirected_input = open(job->input, O_RDONLY);
	dup2(redirected_input, STDIN_FILENO);
	close(redirected_input);
}

void	executor_output(t_jobs *job, t_env *env)
{
	int	redirected_output;

	if (job->output && (job->output[0] == '$'))
	{
		ft_printf_fd(2, "minishell: %s: ambiguous redirect\n", job->output);
		job->output = ft_strdup("/dev/null");
		env->status = 1;
	}
	job->output = unquote_and_direct(job->output, env);
	if (job->append)
		redirected_output = open(job->output, O_CREAT | O_APPEND | O_RDWR, 0644);
	else
	{
		if (access(job->output, F_OK) == 0)
			remove(job->output);
		redirected_output = open(job->output, O_CREAT | O_RDWR, 0644);
	}
	if (ft_strcmp("/dev/null", job->output) == 0)
		env->status = 1;
	dup2(redirected_output, STDOUT_FILENO);
	close(redirected_output);//else
}

void	start_executor(t_jobs *job, t_env *env)
{
//	signal(SIGINT, handle_signal_child);
//	signal(SIGQUIT, sigquit);
	env->saved_stdin = dup(STDIN_FILENO);
	env->saved_stdout = dup(STDOUT_FILENO);
	env->pids = ft_calloc_pids(job);//error check
	while (job)
	{
		//expanding
		if (job->job)
			modify_array(job->job, env);
		//redirections
		job->redir_error_flag = false;
		if (job->input)
			executor_input(job, env);
		if (job->output)
			executor_output(job, env);

		
		
		//executing jobs
		if (job->next && job->next->type == PIPE)
		{
			job->piped = true;
			child_process(job, env);
			job = job->next->next;
			env->status = 0;
			continue;
		}
		//is checking job.job[0][0] correct?
		else if (job->job && job->piped)
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
			job->piped = false;
		}
		else if (job->next && job->next->type == OR)
		{
			if (env->status == 0)
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
			job->piped = false;
		}
		else
			job = job->next;
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
		env->status = WEXITSTATUS(status);
		i++;
	}
	// TODO function for this
	close(env->saved_stdin);
	close(env->saved_stdout);
	free (env->pids);
	return ;
}
