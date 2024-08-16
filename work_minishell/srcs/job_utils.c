/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:20:43 by ycantin           #+#    #+#             */
/*   Updated: 2024/08/16 01:15:13 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	count_jobs(t_token *tokens)
{
    int		num;
    t_token	*cur;

    num = 0;
    cur = tokens;
    while (cur)
    {
        if (cur->type == WORD)
        {
            cur = cur->next;
            while (cur && cur->token[0] == '-')      
                cur = cur->next;
            num++;
            if (!cur) 
                break;
        }
        if (cur && (cur->type != WORD))
            num++;
        cur = cur->next;
    }
    return (num);
}

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
