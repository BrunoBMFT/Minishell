/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frees.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 15:18:01 by ycantin           #+#    #+#             */
/*   Updated: 2024/08/17 16:48:07 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	clean_up_build(t_token **list, char *cmd_line)
{
	clear_list(list);
	free(cmd_line);
}

void	clean_exit(t_jobs *jobs, char *line, char *prompt)
{
	if (jobs)
		clear_jobs(&jobs);
	else
		(void)jobs;
	if (line)
		free (line);
	else
		(void)line;
	if (prompt)
		free (prompt);
	else
		(void)prompt;
	exit (0);
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
		free(current);
		current = temp;
	}
	*lst = NULL;
}
/* yohan's version
void	clear_jobs(t_jobs **lst)
{
	t_jobs	*temp;
	t_jobs	*current;

	current = *lst;
	while (current)
	{
		temp = current->next;
		free_array(current->job);
        free(current);
		current = temp;
	}
	*lst = NULL;
} */

void	free_all(t_token **list, char **array, char *message, int len)
{
	free_array(array);
	clear_list(list);
	write(1, message, len);
	exit(1);
}
