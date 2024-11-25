/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redirs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 16:40:37 by bruno             #+#    #+#             */
/*   Updated: 2024/11/24 17:45:29 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

void	start_pipe(t_jobs **job, t_env *env)
{
	t_jobs *temp1;//make func
	t_jobs *temp2;//make func

	temp1 = (*job);//make func
	temp2 = (*job)->next;//make func
	env->piped = true;
	piped_process((*job), env);
	*job = (*job)->next->next;
	clear_single_job(&temp1);//make func
	clear_single_job(&temp2);//make func
}
