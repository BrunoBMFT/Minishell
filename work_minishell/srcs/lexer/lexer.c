/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 00:13:28 by bruno             #+#    #+#             */
/*   Updated: 2024/10/31 17:14:59 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*developed_cmdline_tokenization(char *command_line)
{
	char	*simplified;
	t_token	*list;

	list = NULL;
	simplified = split_complex_args(command_line);
	tokenize(&list, simplified);
	free(simplified);
	t_token *temp = list;
	while (temp)
	{
		if (temp->type >= 4 && temp->type <= 7 && !temp->next)
		{
			ft_printf_fd(2, "Minishell: syntax error near unexpected token `newline'\n");
			free(command_line);
			clear_list(&list);
			return (NULL);
		}
		temp = temp->next;
	}
	
	return (list);
}

t_jobs	*build(char *command_line, t_env *env)
{
	t_jobs	*jobs;
	t_token	*list;
	t_token	*last;

	if (!command_line[0])
		return (NULL);
	jobs = NULL;
	list = NULL;
	last = NULL;
	list = developed_cmdline_tokenization(command_line);
	if (!list)
		return (NULL);
	if (parse(&list) == -1)
	{
		free(command_line);
		clear_list(&list);
		return (NULL);
	}
	last = get_last_tok(list);
	while (last && last->type >= PIPE && last->type <= OR && !last->next)
		if (parse_last_token(&command_line, &list, &last) == -1)
			return (NULL);
	make_job_list(&jobs, &list, env);
	clear_list(&list);
	free(command_line);
	return (jobs);
}
void	apply_redir(t_token *current, t_jobs *job, t_env *env)
{
	int	fd;

	if (current->type == HEREDOC)
	{
		job->heredoc = 1;
		if (job->delimiters)
			free(job->delimiters);
		job->delimiters = ft_strdup(current->next->token);
		if (job->input)
			free(job->input);
		job->input = ft_strdup(job->heredoc_file);
		if (handle_heredoc(job, *env) < 0)
			printf ("error handling heredocs\n");
	}
	if (current->type == INPUT)
	{
		if (job->input)
		{
			job->mult_input_flag = 1;
			free(job->input);
		}
		current->next->token = unquote_and_direct(current->next->token, env);
		if (access(current->next->token, F_OK) != 0)
		{
			if (!env->redir_error_flag)//replace with status flag
				ft_printf_fd(2, "bash: %s: No such file or directory\n", current->next->token);
			env->redir_error_flag = true;
			job->input = ft_strdup("/dev/null");
		}
		else if (job->input && (job->input[0] == '$'))
		{
			if (!env->redir_error_flag)//replace with status flag
				ft_printf_fd(2, "minishell: %s: ambiguous redirect\n", job->input);
			env->redir_error_flag = true;
			job->input = ft_strdup("/dev/null");
		}
		else 
			job->input = ft_strdup(current->next->token);
	}
	if (current->type == OUTPUT || current->type == APPEND_OUT)
	{
		current->next->token = unquote_and_direct(current->next->token, env);
		fd = open(current->next->token, O_CREAT | O_RDWR, 0644);
		close(fd);
		if (current->type == APPEND_OUT)
			job->append = 1;
		if (job->output)
			free(job->output);
		job->output = ft_strdup(current->next->token);
	}
}

char	**job_array(t_token **cur, t_jobs **job, t_env *env)
{
	int		i;
	char	**array;

	i = 0;
	array = malloc(sizeof(char *) * (count_tokens_in_job(*cur) + 1));
	if (!array)
		return (NULL);
	while (*cur && (*cur)->type != AND && (*cur)->type != OR
		&& (*cur)->type != PIPE)
	{
		if ((*cur)->type >= INPUT && (*cur)->type <= APPEND_OUT)
		{
			apply_redir(*cur, *job, env);
			*cur = (*cur)->next->next;
		}
		else
		{
			array[i++] = ft_strdup((*cur)->token);
			*cur = (*cur)->next;
		}
	}
	if (i == 0)
		return (free(array), NULL);
	array[i] = NULL;
	return (array);
}

char *filename(int i)
{
	char *num;
	char *full;

	num = ft_itoa(i);
	full = ft_strjoin(".heredoc_", num);
	free(num);
	return (full);
}

void	make_job_list(t_jobs **job_list, t_token **tok_list, t_env *env)
{
	t_token	*cur;
	t_jobs	*new;
	int		i;

	i = 0;
	env->redir_error_flag = false;
	cur = *tok_list;
	while (cur)
	{
		new = addjob(NULL);
		if (cur && (cur->type == PIPE || cur->type == AND || cur->type == OR))
		{
			if (cur->type > 0 && cur->type < 4)
			{
				new->type = cur->type;
				new->job = NULL;
			}
			go_to_next_job(job_list, new);
			cur = cur->next;
			continue ;
		}
		if (new->heredoc_file)
    		free(new->heredoc_file);
		new->heredoc_file = filename(i);
		new->job = job_array(&cur, &new, env);
		new->type = WORD;
		go_to_next_job(job_list, new);
		i++;
	}
}
