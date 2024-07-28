/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 17:26:33 by bruno             #+#    #+#             */
/*   Updated: 2024/07/25 00:34:33 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


int	start_executor(t_jobs *job, char **env, char **temp_vars)
{
	int status = try_builtins(job, env, temp_vars);
//	printf("final status: %d\n", status);
	
/* 	while (job && job->next)// && job->next
	{
//		printf("cmd: %s\t  execd: %s\t  type:%d\n", job->cmd, job->execd, job->type);
//		run_execution(job, env);
 		if (job->next->type == PIPE)
		{ 
 			if (try_builtins(job, env) == 200)
				printf("executor\n");
			printf("processing pipe\n");
			child_process(job, env);
			job = job->next;
		}
		else if (job->next->type == AND)
		{
 			if (try_builtins(job, env) == 200)
				printf("executor\n")
			simple_process(job, env);
			job = job->next;
		}
//		if (job->type == HEREDOC)
//			start_heredoc(job);
 		else if (job->next->type == OR)//(||) not working for now cause of last process running
		{
			if (simple_process(job, env) != 0)
			{
				job = job->next;
				simple_process(job, env);
			}
		}
		job = job->next;
	}
	int status = simple_process(job, env);
//	printf("final status: %d\n", status);*/
	return (1);
}

/* void	run_execution(t_jobs *job, char **env)//
{
	if (job->next->type == 1)//(|)
	{
		printf("processing pipe\n");
		child_process(job, env);
		job = job->next;
		job = job->next;
	}
	if (job->next->type == 2)//(&&)
	{
		simple_process(job, env);
		job = job->next;
	}
	if (job->next->type == 3)//(||) not working for now cause of last process running
	{
		if (simple_process(job, env) != 0)
		{
			job = job->next;
			simple_process(job, env);
		}
	}
} */
