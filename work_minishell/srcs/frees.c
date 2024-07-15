/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frees.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 15:18:01 by ycantin           #+#    #+#             */
/*   Updated: 2024/07/10 22:08:28 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
		if (current->cmd)
			free(current->cmd);
		if (current->execd)
			free(current->execd);
		if (current->job)
			free_array(current->job);
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
