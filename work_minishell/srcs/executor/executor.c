/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 17:26:33 by bruno             #+#    #+#             */
/*   Updated: 2024/09/27 01:54:36 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		choose_input(t_jobs **jobs)
{
	t_jobs *copy;
	int 	input;

	copy = *jobs;
	while (copy)
	{
		if (handle_heredoc(copy) < 0)
			return (-1);
	}
}

//errors should return
int	start_executor(t_jobs *job, t_env env)
{
	int 	status = 0;
	int 	saved_stdin = dup(STDIN_FILENO);
	int 	saved_stdout = dup(STDOUT_FILENO);
	int 	redirected_input;
	int 	redirected_output;
	bool	piped = false;// TODO check if works
/* 	signal(SIGINT, handle_signal_child);
	signal(SIGQUIT, sigquit); */
	
	int i = 0;
	while (job)
	{
		if (job->job)
			modify_array(job->job, env);
		//redirections
		if (job->input)
        {
            redirected_input = open(job->input, O_RDONLY);
            if (redirected_input < 0) {
                perror("Failed to open input file");
                status = 127;
            } else if (dup2(redirected_input, STDIN_FILENO) < 0) {
                perror("dup2 failed for input");
                status = 127;
            }
            close(redirected_input);
        }
		if (job->output)
        {
            if (job->append)
                redirected_output = open(job->output, O_CREAT | O_APPEND | O_RDWR, 0644);
            else
            {
                if (access(job->output, F_OK) == 0)
                    remove(job->output);
                redirected_output = open(job->output, O_CREAT | O_RDWR, 0644);
            }

            if (redirected_output < 0) {
                perror("Failed to open output file");
                status = 127;
            } else if (dup2(redirected_output, STDOUT_FILENO) < 0) {
                perror("dup2 failed for output");
                status = 127;
            }
            close(redirected_output);
        }
		//pipes
		if (job->next && job->next->type == PIPE)
		{
			child_process(job, env);
			job = job->next->next;
			piped = true;
			continue;
		}
		//executing jobs
		else if (job->job && job->job[0] && piped)
			status = child_process(job, env);
		else if (job->job && job->job[0])
			status = simple_process(job, env);
		if (dup2(saved_stdin, STDIN_FILENO) < 0 || dup2(saved_stdout, STDOUT_FILENO) < 0) {
            perror("dup2 reset failed");
            status = 127;
        }
		if (job->heredoc_file && access(job->heredoc_file, F_OK) == 0)
			remove(job->heredoc_file);
		if (job->next && job->next->type == AND)
		{
			job = job->next->next;
			piped = false;
		}
		else if (job->next && job->next->type == OR)//need to check pipe priorities
		{
			if (status == 0)
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
			piped = false;
		}
		else
			job = job->next;
	}
	if (access(".heredoc", F_OK) == 0)
		remove(".heredoc");
	close(saved_stdin);
	close(saved_stdout);
	return (status);
}
