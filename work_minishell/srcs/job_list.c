/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:20:43 by ycantin           #+#    #+#             */
/*   Updated: 2024/08/16 01:14:22 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	*developed_cmdline_tokenization(char *command_line, char **env, int status)
{
	char	*converted;
	char	*simplified;
	t_token	*list;

	list = NULL;
	simplified = split_complex_args(command_line);
	converted = unquote_and_direct(simplified, env, NULL, status);
	free(simplified);
	tokenize(&list, converted);
	free(converted);
	if (parse(&list) == -1)
	{
		clear_list(&list);
		return (NULL);
	}
	return (list);
}

t_jobs	*build(char *command_line, char **env, int status)
{
	t_jobs	*jobs;
	t_token	*list;
	t_token	*last;
	t_jobs	*temp;

	jobs = NULL;
	list = NULL;
	last = NULL;
	list = developed_cmdline_tokenization(command_line, env, status);
	if (parse(&list) == -1)
	{
		clear_list(&list);
		return (NULL);
	}
	last = get_last_tok(list);
	while (last && last->type >= PIPE && last->type <= OR && !last->next)
		if (parse_last_token(&command_line, &list, &last) == -1)
			return (NULL);
	make_job_list(&jobs, &list, env);
	temp = jobs;
	clean_up_build(&list, command_line);
	return (jobs);
}

char	*unquoted(char *str, char **env)
{
    char *new;

    if (*str == '\'')
        new = ft_strtrim(str, "\'");
    else if(*str == '\"')
        new = ft_strtrim(expand_env_vars(str, env, NULL), "\"");
    else
        new = ft_strdup(expand_env_vars(str, env, NULL));
    return (new);
}

char	*assign_cmd(t_token *cur)
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
    else if (cur->type == EXPORT)
        return ("export");
    return (NULL);
}

void    apply_redir(t_token *current, t_jobs *job)
{
    if (current->type == INPUT || current->type == HEREDOC)
    {
        if (current->type == HEREDOC)
            job->heredoc = 1;
        if (job->input)
        {
            job->mult_input_flag = 1;
            free(job->input);
        }
        job->input = ft_strdup(current->next->token);
    }
    else if(current->type == OUTPUT || current->type == APPEND_OUT)
    {
        if (current->type == APPEND_OUT)
            job->append = 1;
        if (job->output)
            free(job->output);
        job->output = ft_strdup(current->next->token);
    }
}

char **job_array(t_token **cur, t_jobs **job, char **env)
{
    int i;
    int count;
    char **array;
    t_token *temp;
    
    i = 0;
    count = 0;
    temp = *cur;
    while (temp && temp->type != AND && temp->type != OR && temp->type != PIPE)
    {
        if (temp->type == INPUT || temp->type == OUTPUT || temp->type == HEREDOC || temp->type == APPEND_OUT)
        {
            temp = temp->next->next;
            continue;
        }
        count++;
        temp = temp->next;
    }
    array = malloc(sizeof(char *) * (count + 1));
    if (!array)
        return (NULL);
    while (*cur && (*cur)->type != AND && (*cur)->type != OR && (*cur)->type != PIPE)
    {
        if ((*cur)->type == INPUT || (*cur)->type == OUTPUT || (*cur)->type == HEREDOC || (*cur)->type == APPEND_OUT)
        {
            apply_redir(*cur, *job);
            *cur = (*cur)->next->next;
            continue;
        }
        array[i] = ft_strdup((*cur)->token);
        i++;
        *cur = (*cur)->next;
    }
    array[i] = NULL;
    return (array);
}

void	make_job_list(t_jobs **job_list, t_token **tok_list, char **env)
{
    t_token *cur;
    t_jobs *new;
    char *cmd;

    cur = *tok_list;
    while (cur)
    {
        new = addjob(NULL);
        if (cur && cur->type == PIPE || cur->type == AND || cur->type == OR)
        {
            if (cur->type > 0 && cur->type < 4)
            {
                new->type = cur->type;
                new->job = NULL;
            }
            go_to_next_job(job_list, new);
            cur = cur->next;
            continue;
        }
        new->job = job_array(&cur, &new, env);
        new->type = WORD;
        go_to_next_job(job_list, new);
    }
}
