/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 00:13:28 by bruno             #+#    #+#             */
/*   Updated: 2024/11/08 05:16:08 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	set_input(t_jobs *job, char *new_input)
{
	if (job->input)
		free(job->input);
	job->input = ft_strdup(new_input);
}

void	set_output(t_jobs *job, char *new_output)
{
	if (job->output)
		free(job->output);
	job->output = ft_strdup(new_output);
}

void	apply_heredoc(t_jobs *job, t_token *current, t_env *env)
{
	job->heredoc = 1;
	if (job->delimiters)
		free(job->delimiters);
	job->delimiters = ft_strdup(current->next->token);
	set_input(job, job->heredoc_file);
	if (handle_heredoc(job, *env) < 0)
		ft_printf_fd (2, "error handling heredocs\n");
}

void	apply_output(t_jobs *job, t_token *current, t_env *env)
{
	char	*temp;
	int		fd;

	temp = unquote_and_direct(current->next->token, env);
	free(current->next->token);
	current->next->token = temp;
	fd = open(current->next->token, O_CREAT | O_RDWR, 0644);
	close(fd);
	if (current->type == APPEND_OUT)
		job->append = 1;
	set_output(job, current->next->token);
}

void	apply_redir(t_token *current, t_jobs *job, t_env *env)
{
	int		fd;
	char	*temp;

	if (current->type == HEREDOC)
		apply_heredoc(job, current, env);
	if (current->type == INPUT)
	{
		temp = unquote_and_direct(current->next->token, env);
		free(current->next->token);
		current->next->token = temp;
		if (access(current->next->token, F_OK) != 0)
		{
			if (!env->redir_error_flag)
				ft_printf_fd(2, "bash: %s: No such file or directory\n", current->next->token);
			env->redir_error_flag = true;
			set_input(job, "/dev/null");
		}
		else if (job->input && (job->input[0] == '$'))
		{
			if (!env->redir_error_flag)
				ft_printf_fd(2, "minishell: %s: ambiguous redirect\n", job->input);
			env->redir_error_flag = true;
			set_input(job, "/dev/null");
		}
		else
			set_input(job, current->next->token);
	}
	if (current->type == OUTPUT || current->type == APPEND_OUT)
		apply_output(job, current, env);
}
