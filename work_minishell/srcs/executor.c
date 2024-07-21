/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 17:26:33 by bruno             #+#    #+#             */
/*   Updated: 2024/07/21 17:19:06 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* struct cmd *pipecmd(struct cmd *left, struct cmd *right)
{
	struct pipecmd *cmd;

	cmd = malloc(sizeof(*cmd));
	memset(cmd, 0, sizeof(*cmd));
	cmd->type = PIPE;
	cmd->left = left;
	cmd->right = right;
	return (struct cmd *)cmd;
} */
//Variable names in the shell should consist of alphanumeric characters and underscores (_). They should not start with a digit.
char	**variable_declaration(char *str, char **vars)//will work with expand_vars
{//if the var_name is the same, remove the first
//hi=mi hello=world has to work
	int i = 0;
	char **temp_vars;
	int	var_count = ft_split_wordcount(vars);

	temp_vars = ft_calloc(sizeof(char *), var_count + 2);//broken
	if (!temp_vars)
		panic("malloc\n");
	while (i < var_count)
	{
		temp_vars[i] = ft_strdup(vars[i]);
//		free (vars[i]);//check
		i++;
	}
	temp_vars[i] = ft_strdup(str);
	i = 0;
	while (temp_vars[i])
	{
		printf("%d: %s\n", i, temp_vars[i]);
		i++;
	}
//	free_array(temp_vars);
	vars = temp_vars;
	return (temp_vars);
}

int	start_executor(t_jobs *job, char **env)
{
	while (job && job->next)//pipes, && and ||
	{
//		printf("cmd: %s\t  execd: %s\t  type:%d\n", job->cmd, job->execd, job->type);
/* 		if (try_builtins(job, env) == 200)
			run_execution(job, env); */
		if (job->next->type == 1)
		{
//			printf("processing pipe\n");
			child_process(job, env);
			job = job->next;
		}
		else if (job->next->type == 2)//(&&)
		{
			simple_process(job, env);
			job = job->next;
		}
/* 		else if (job->next->type == 3)//(||) not working for now cause of last process running
		{
			if (simple_process(job, env) != 0)
			{
				job = job->next;
				simple_process(job, env);
			}
		} */
		if (job->next)
			job = job->next;
	}
//	printf("cmd: %s\t  execd: %s\t  type:%d\n", job->cmd, job->execd, job->type);
	int status = simple_process(job, env);
//	printf("final status: %d\n", status);
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
