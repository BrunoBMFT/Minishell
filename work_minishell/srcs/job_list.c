/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:20:43 by ycantin           #+#    #+#             */
/*   Updated: 2024/07/21 18:02:53 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char *full_cmd(t_token **cur)
{
    char *job;
    char *temp;

    if (!(*cur) || (*cur)->type != WORD)
        return NULL;

    job = ft_strdup((*cur)->token);
    *cur = (*cur)->next;

    while (*cur && (*cur)->type == WORD && (*cur)->token[0] == '-')
    {
        temp = ft_strjoin(job, " ");
        free(job);
        job = ft_strjoin(temp, (*cur)->token);
        free(temp);
        *cur = (*cur)->next;
    }
    return job;
}

char *get_execd(t_token **cur)
{
    char *execd;
    char *temp;
    
    execd = ft_strdup((*cur)->token);
    *cur = (*cur)->next;

    while (*cur && (*cur)->type == WORD)
    {
        if (!(*cur) || (*cur)->type != WORD)
            break;
        temp = ft_strjoin(execd, " ");
        free(execd);
        execd = ft_strjoin(temp, (*cur)->token);
        free(temp);
        *cur = (*cur)->next;
    }
    return (execd);
}

char **job_array(t_jobs *node)
{
	char **array;
	char **new_array;
	int i;

	array = ft_split(node->cmd, ' ');
	i = count_strings(array);
	new_array = malloc(sizeof(char *) * (i + 2));
	if (!new_array)
	{
		free(array);
		return(NULL);
	}
	i = 0;
	while (array[i])
	{
		new_array[i] = ft_strdup(array[i]);
		i++;
	}
	if (node->execd)//edited by bruno
	{//edited by bruno
		new_array[i] = ft_strdup(node->execd);//edited by bruno
		new_array[i + 1] = NULL;//edited by bruno
	}//edited by bruno
	else//edited by bruno
        new_array[i] = NULL;//edited by bruno
	free_array(array);
	return (new_array);
}

char *assign_cmd(t_jobs *cur)
{
    if (cur->type == AND)
        return ("&&");
    else if (cur->type == OR)
        return ("||");
    else if (cur->type == PIPE)
        return ("|");   
    else if (cur->type == APPEND_OUT)
        return (">>");   
    else if (cur->type == HEREDOC)
        return ("<<");   
    else if (cur->type == INPUT)
        return ("<");   
    else if (cur->type == OUTPUT)
        return (">");  
    return (NULL);
}

void handle_non_word_tokens(t_jobs **cur, t_token **tok_cur)
{
    if ((*tok_cur)->type >= 4 && (*tok_cur)->type <= 7 && (*tok_cur)->token)
    {
        char *next;

        next = (*tok_cur)->next->token;
        (*cur)->type = (*tok_cur)->type;
        (*cur)->cmd = ft_strdup(assign_cmd(*cur));
        (*cur)->execd = ft_strdup(next);
        (*cur)->job = job_array(*cur);
        *tok_cur = (*tok_cur)->next;
    }
    else if ((*tok_cur)->type > 0 && (*tok_cur)->type < 4)
    {
        (*cur)->type = (*tok_cur)->type;
        (*cur)->cmd = ft_strdup(assign_cmd(*cur));
        (*cur)->execd = NULL;
        (*cur)->job = NULL;
    }
}

void make_job_list(t_jobs **job_list, t_token **tok_list)
{
    t_token *cur;
    t_jobs *new;
    char *cmd;

    cur = *tok_list;
    while (cur)
    {
        new = addjob(NULL);
        if (cur && cur->type != WORD)
        {
            handle_non_word_tokens(&new, &cur); //deals with redirections, &&, | and ||
            go_to_next_job(job_list, new);
            cur = cur->next;
            continue;
        }
        cmd = full_cmd(&cur);
        new->cmd = ft_strdup(cmd);
        new->type = WORD;
        free(cmd);
        if (cur && cur->type == WORD)
            new->execd = get_execd(&cur);
        new->job = job_array(new);
        go_to_next_job(job_list, new);
    }
}
