/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frees.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 15:18:01 by ycantin           #+#    #+#             */
/*   Updated: 2024/11/13 16:45:36 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	clean_exit(t_jobs *jobs, t_env *env, int status)
{
	if (jobs->job)
		clear_jobs(&jobs);
	if (env->saved_stdin)
		close (env->saved_stdin);
	if (env->saved_stdout)
		close (env->saved_stdout);
	if (env->env)
		free_array(env->env);
	if (env->pids)
		free (env->pids);
	rl_clear_history();
	exit (status);
}

void	clean_up_build(t_token **list, char *cmd_line)
{
	clear_list(list);
	free(cmd_line);
}

void	clear_list(t_token **lst)
{
	t_token	*temp;
	t_token	*current;

	current = *lst;
	while (current)
	{
		temp = current->next;
		free(current->token);
		free(current);
		current = temp;
	}
	*lst = NULL;
}

void	clear_jobs(t_jobs **lst)
{
	t_jobs	*temp;
	t_jobs	*current;

	current = *lst;
	while (current)
	{
		temp = current->next;
		if (current->job)
			free_array(current->job);
		if (current->heredoc_file)
			free(current->heredoc_file);
		if (current->input)
			free(current->input);
		if (current->output)
			free(current->output);
		if (current->delimiters)
			free(current->delimiters);
		free(current);
		current = temp;
	}
	*lst = NULL;
}

void	free_all(t_token **list, char **array, char *message, int len)
{
	free_array(array);
	clear_list(list);
	write(1, message, len);
	exit(1);
}
