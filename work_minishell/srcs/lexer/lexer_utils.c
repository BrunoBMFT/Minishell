/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:20:43 by ycantin           #+#    #+#             */
/*   Updated: 2024/09/13 17:01:06 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_jobs	*addjob(void *content)
{
	t_jobs	*list;

	list = malloc(sizeof(t_jobs));
	if (!list)
		return (NULL);
	list->job = NULL;
	list->type = 0;
	list->append = 0;
	list->heredoc = 0;
	list->heredoc_file = NULL;
	list->delimiters = NULL;
	list->mult_input_flag = 0;
	list->input = NULL;
	list->output = NULL;
	list->next = NULL;
	return (list);
}

t_jobs	*get_last_job(t_jobs *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	go_to_next_job(t_jobs **lst, t_jobs *new)
{
	t_jobs	*current;

	if (!new)
		return ;
	current = get_last_job(*lst);
	if (lst && current)
		current->next = new;
	else
		*lst = new;
}

int	count_tokens_in_job(t_token *cur)
{
	t_token	*temp;
	int		count;

	count = 0;
	temp = cur;
	while (temp && temp->type != AND && temp->type != OR && temp->type != PIPE)
	{
		if (temp->type >= 4 && temp->type <= APPEND_OUT)
			temp = temp->next->next;
		else
		{
			count++;
			temp = temp->next;
		}
	}
	return (count);
}