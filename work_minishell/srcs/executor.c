/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 17:26:33 by bruno             #+#    #+#             */
/*   Updated: 2024/08/08 23:13:56 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	test(t_jobs *job, char **env)
{
	int status;
	while (job){
/* 		if (job->next && job->next->type == APPEND_OUT){
			status = append_to_file(job, env);
            job = job->next->next;
		}
		else if (job->next && job->next->type == OUTPUT){
			status = update_output(job, env);
            job = job->next->next;
		}
		else if (job->type == INPUT){
			update_input(job);
			job = job->next;
		}
		else if (job->type == HEREDOC){
			start_heredoc(job);
			job = job->next;
		} */
		if (job->next && job->next->type == PIPE){
			//child process
			job = job->next->next;
		}
		else if (job->next && job->next->type == AND){
			//simple process
			job = job->next;
		}
		else{
			//simple process
            job = job->next;
        }
	}
}
// ! START TESTING ERRORS
//inputs and outputs (< and >) can happen at any place
int	start_executor(t_jobs *job, char **env, char **temp_vars)
{
	int status = 0;
	int stdin = dup(STDIN_FILENO);
	while (job)
//	return (print_jobs(job), 0);
	{
		if (job->next && job->next->type == PIPE) {
			child_process(job, env, temp_vars);
			job = job->next->next;
		}
/* 		if (job->next && job->next->type == APPEND_OUT)
		{
			status = append_to_file(job, env, temp_vars);
			dup2(stdin, STDIN_FILENO);
            job = job->next->next;
		} */
		else if (job->next && job->next->type == OUTPUT)
		{
			status = update_output(job, env, temp_vars);
			dup2(stdin, STDIN_FILENO);
            job = job->next->next;
		}
		else if (job->type == INPUT)
		{
			if (update_input(job) == 0)//if command requires an infile but infile fails, it breaks
				job = job->next;
		}
/* 		else if (job->type == HEREDOC)
		{
			start_heredoc(job);
			job = job->next;
		} */
		else if (job->next && job->next->type == AND) {
			simple_process(job, env, temp_vars);
			dup2(stdin, STDIN_FILENO);
			job = job->next->next;
		}
		else {
			status = simple_process(job, env, temp_vars);//dont like temp_vars here but it has to be?
			dup2(stdin, STDIN_FILENO);
			job = job->next;
		}
	}
//	printf("status: %d\n", status);
	close (stdin);
	return (1);
}

