/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:20:43 by ycantin           #+#    #+#             */
/*   Updated: 2024/07/21 18:03:03 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int give_type(t_token *token)
{
    if(token->type == PIPE)
        return (PIPE);
    else if(token->type == AND)
        return (AND);
    else if(token->type == OR)
        return (OR);
    else if(token->type == INPUT)
        return (INPUT);
    else if(token->type == OUTPUT)
        return (OUTPUT);
    else if(token->type == APPEND_OUT)
        return (APPEND_OUT);
    else if(token->type == HEREDOC)
        return (HEREDOC);
    return (0);
}

int count_jobs(t_token *tokens)
{
    int num;
    t_token *cur;

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
    list->cmd = NULL;
    list->execd = NULL;
    list->job = NULL;
    list->type = 0;
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