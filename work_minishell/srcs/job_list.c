/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:20:43 by ycantin           #+#    #+#             */
/*   Updated: 2024/08/10 18:44:56 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	*developed_cmdline_tokenization(char *command_line, char **env)
{
	char	*converted;
	char	*simplified;
	t_token	*list;

	list = NULL;
	simplified = split_complex_args(command_line);
	converted = unquote_and_direct(simplified, env, NULL);
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

t_jobs	*build(char *command_line, char **env)
{
	t_jobs	*jobs;
	t_token	*list;
	t_token	*last;
	t_jobs	*temp;

	jobs = NULL;
	list = NULL;
	last = NULL;
	list = developed_cmdline_tokenization(command_line, env);
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
	char	*new;

	if (*str == '\'')
		new = ft_strtrim(str, "\'");
	else if (*str == '\"')
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

char	**job_array(t_token **cur, char **env)
{
	int		i;
	int		count;
	char	**array;
	t_token	*temp;

	i = 0;
	count = 0;
	temp = *cur;
	while (temp && temp->type == WORD)
	{
		count++;
		temp = temp->next;
	}
	array = malloc(sizeof(char *) * (count + 1));
	if (!array)
		return (NULL);
	while (*cur && (*cur)->type == WORD)
	{
		array[i] = ft_strdup((*cur)->token);
		i++;
		*cur = (*cur)->next;
	}
	array[i] = NULL;
	return (array);
}

char	**redir_array(t_token **cur, char **env)
{
	char	**array;

	array = malloc(sizeof (char *) * 2);
	if (!array)
		return (NULL);
	array[0] = ft_strdup((*cur)->next->token);
	array[1] = NULL;
	return (array);
}

void	handle_non_word_tokens(t_jobs **cur, t_token **tok_cur, char **env)
{
	char	*next;

	if ((*tok_cur)->type >= 4 && (*tok_cur)->type <= 7 && (*tok_cur)->token)
	{
		next = (*tok_cur)->next->token;
		(*cur)->type = (*tok_cur)->type;
		(*cur)->job = redir_array(tok_cur, env);
		*tok_cur = (*tok_cur)->next;
	}
	else if ((*tok_cur)->type > 0 && (*tok_cur)->type < 4)
	{
		(*cur)->type = (*tok_cur)->type;
		(*cur)->job = NULL;
	}
}

void	make_job_list(t_jobs **job_list, t_token **tok_list, char **env)
{
	t_token	*cur;
	t_jobs	*new;
	char	*cmd;

	cur = *tok_list;
	while (cur)
	{
		new = addjob(NULL);
		if (cur && cur->type != WORD)
		{
			handle_non_word_tokens(&new, &cur, env);
			go_to_next_job(job_list, new);
			cur = cur->next;
			continue ;
		}
		new->job = job_array(&cur, env);
		new->type = WORD;
		go_to_next_job(job_list, new);
	}
}
