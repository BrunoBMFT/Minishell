/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 17:26:33 by bruno             #+#    #+#             */
/*   Updated: 2024/08/15 20:45:08 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
//execd is job[1], check if everything works
//cmd is job[0]
//inputs and outputs (< and >) can happen at any place
int	start_executor(t_jobs *job, char **env, char ***temp_vars)
{
	int status = 0;
	int saved_stdin = dup(STDIN_FILENO);
/* 	if (set_signal(SIGINT, ctrl_c_idle) < 0 || set_signal(SIGQUIT, sigquit) < 0)
	{
		ft_printf("Error: signal\n");
		clear_jobs(&job);
		free_array(env);
		exit(1);
	} */
	while (job)
	{//change things to else ifs
		if (job->next && job->next->type == APPEND_OUT)
		{
			status = append_to_file(job, env, temp_vars);
			dup2(saved_stdin, STDIN_FILENO);
            job = job->next->next;
		}
		else if (job->next && job->next->type == OUTPUT)
		{
			status = update_output(job, env, temp_vars);
			dup2(saved_stdin, STDIN_FILENO);
            job = job->next->next;
		}
		else if (job->type == HEREDOC)
		{
			start_heredoc(job);
			job = job->next;
		}
		else if (job->type == INPUT)
		{
			update_input(job);//if command requires an infile but infile fails, it breaks
			job = job->next;
		}
		else if (job->next && job->next->type == PIPE) {
			child_process(job, env, temp_vars);
			job = job->next->next;
		}
		else if (job->next && job->next->type == AND) {
			simple_process(job, env, temp_vars);
			dup2(saved_stdin, STDIN_FILENO);
			job = job->next->next;
		}
		else if (job->next && job->next->type == OR)
		{
			status = simple_process(job, env, temp_vars);
			dup2(saved_stdin, STDIN_FILENO);
			if (status == 0)
			{
				while (job->next && job->next->type == OR)
					job = job->next;
				if (job->next)
					job = job->next->next;
			}
			else
				job = job->next->next;
		}
		else {
			status = simple_process(job, env, temp_vars);//dont like temp_vars here but it has to be?
			dup2(saved_stdin, STDIN_FILENO);
			job = job->next;
		}
	}
//	printf("status: %d\n", status);
	close (saved_stdin);
	return (status);
}

