/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 17:26:33 by bruno             #+#    #+#             */
/*   Updated: 2024/07/20 17:24:03 by bruno            ###   ########.fr       */
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
{
	int i = 0;
	char **temp_vars;
	int	var_count = ft_split_wordcount(vars);

	temp_vars = ft_calloc(sizeof(char *), var_count + 2);//broken
	if (!temp_vars)
		panic("malloc\n");
	while (i < var_count)
	{
		temp_vars[i] = ft_strdup(vars[i]);
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
	while (job)//pipes, && and ||
	{
//		printf("cmd: %s\t  execd: %s\t  type:%d\n", job->cmd, job->execd, job->type);
		if (try_builtins(job, env) == 200)
			run_execution(job, env);
		job = job->next;
	}
	return (1);
}

void	run_execution(t_jobs *curr, char **env)//
{
//	printf("executor\n");
//	printf("cmd: %s\t  execd: %s\t  type:%d\n", curr->cmd, curr->execd, curr->type);
//	execute_builtins(curr, env);
/*	printf("cmd: %s\t  execd: %s\t  type:%d\n", curr->cmd, curr->execd, curr->type);
	if (curr->next->type == 1)//(|)
	{
		printf("processing pipe\n");
		child_process(curr, env);
		curr = curr->next;
		curr = curr->next;
	}
	if (curr->next->type == 2)//(&&)
	{
		simple_process(curr, env);
		curr = curr->next;
	}
	if (curr->next->type == 3)//(||) not working for now cause of last process running
	{
		if (simple_process(curr, env) != 0)
		{
			curr = curr->next;
			simple_process(curr, env);
		}
	}
//		child_process(curr, env);
//		else
//			status = simple_process(curr, env); */
//	status = simple_process(curr, env);
//	printf("final status: %d\n", status);
}
