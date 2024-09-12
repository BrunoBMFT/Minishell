/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brfernan <brfernan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 17:26:33 by bruno             #+#    #+#             */
/*   Updated: 2024/09/12 18:21:47 by brfernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
//errors should return
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
	int i = 0;
	while (job)
	{
		modify_array(job->job, env);
		if (job->heredoc)
			if ((redirected_input = handle_heredoc(job)) < 0)
					return (127);
		if (job->input)
		{
			redirected_input = open(job->input, O_RDONLY);
			if (redirected_input < 0)
			{
				ft_printf_fd(2, "minishell: %s: No such file or directory\n", job->input);
				return (127);
			}
			if (dup2(redirected_input, STDIN_FILENO) < 0)
				status = 127;
			close (redirected_input);
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
		if (job->next && job->next->type == PIPE)
		{
			child_process(job, env);
			job = job->next->next;
			piped = true;
			continue;
		}

		else if (job->job && job->job[0] && piped)
			status = child_process(job, env);
			
		else if (job->job && job->job[0])
			status = simple_process(job, env);
		if (dup2(saved_stdin, STDIN_FILENO) < 0 || dup2(saved_stdout, STDOUT_FILENO) < 0)
			status = 127;
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
