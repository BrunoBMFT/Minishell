/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brfernan <brfernan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 17:26:33 by bruno             #+#    #+#             */
/*   Updated: 2024/10/10 12:24:08 by brfernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Yohan's!
int	start_executor(t_jobs *job, t_env env)
{
	int 	status = 0;
	int 	saved_stdin = dup(STDIN_FILENO);
	int 	saved_stdout = dup(STDOUT_FILENO);
	int 	redirected_input;
	int 	redirected_output;
	bool	piped = false;// TODO check if works
	signal(SIGINT, handle_signal_child);
	signal(SIGQUIT, sigquit);
	while (job)
	{
		print_jobs(job);
		if (job->input)
		{
			if (job->heredoc)
			{
				if ((redirected_input = handle_heredoc(job)) < 0)
					return (127);
			}
			else
			{
				redirected_input = open(job->input, O_RDONLY);
				if (redirected_input < 0)
					perror("input file error\n");
				if (dup2(redirected_input, STDIN_FILENO) < 0)
					status = 127;
				close (redirected_input);
			}
		}
		if (job->next && job->next->type == PIPE)
		{
			child_process(job, env);
			job = job->next->next;
			piped = true;
			continue;
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
			if (redirected_output < 0)
				perror("output file error\n");
			if (dup2(redirected_output, STDOUT_FILENO) < 0)
				status = 127;
			close(redirected_output);
		}
		if (!piped && ft_strcmp(job->job[0], "exit") == 0)
			caught_exit(job, env);
		else if (job->job && job->job[0] && !piped)
			status = simple_process(job, env);
		else if (job->job && job->job[0])
			status = child_process(job, env);
		if (dup2(saved_stdin, STDIN_FILENO) < 0 || dup2(saved_stdout, STDOUT_FILENO) < 0)
			status = 127;
		if (job->next && job->next->type == AND)
		{
			job = job->next->next;
			piped = false;// ! FIX URGENTLY
		}
		else if (job->next && job->next->type == OR)
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




/* 
int	start_executor(t_jobs *job, char **env, char ***temp_vars)
{
	int	status;
	int	saved_stdin;

	saved_stdin = dup(STDIN_FILENO);
	status = 0;	
	 	if (set_signal(SIGINT, ctrl_c_idle) < 0 || set_signal(SIGQUIT, sigquit) < 0)
	{
		ft_printf("Error: signal\n");
		clear_jobs(&job);
		free_array(env);
		exit(1);
	}
	while (job)
	{
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
			handle_heredoc(job->input);
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
			status = simple_process(job, env, temp_vars);
			dup2(saved_stdin, STDIN_FILENO);
			job = job->next;
		}
	}
	close (saved_stdin);
	return (status);
}
*/