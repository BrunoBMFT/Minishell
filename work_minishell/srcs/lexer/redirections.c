/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 14:46:14 by bruno             #+#    #+#             */
/*   Updated: 2024/11/22 10:42:12 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

void	redir_error(t_jobs *job, t_env *env, char *str, t_redir_type type)
{
	if (env->status != 1)
	{
		if (type == REDIR_IN_AMBIGUOUS || type == REDIR_OUT_AMBIGUOUS)
			ft_printf_fd(2, "minishell: %s: ambiguous redirect\n", str);
		else if (type == REDIR_IN_NO_FILE || type == REDIR_OUT_NO_FILE)
			ft_printf_fd(2, "minishell: %s: No such file or directory\n", str);
	}
	env->status = 1;
	if (type == REDIR_IN_AMBIGUOUS || type == REDIR_IN_NO_FILE)
		job->input = ft_strdup("/dev/null");
	else
		job->output = ft_strdup("/dev/null");
}

void	apply_output(t_jobs *job, t_token *current, t_env *env)
{
	char	*temp;
	int		fd;

	if (current->next->token && (current->next->token[0] == '$'))
		return (redir_error(job, env, current->next->token,
				REDIR_OUT_AMBIGUOUS), (void) NULL);
	temp = unquote_and_direct(current->next->token, env);
	free(current->next->token);
	current->next->token = temp;
	fd = open(current->next->token, O_CREAT | O_RDWR, 0644);
	if (fd > 0)
		close(fd);
	if (access(current->next->token, F_OK) != 0)
		redir_error(job, env, current->next->token, REDIR_OUT_NO_FILE);
	else
	{
		if (current->type == APPEND_OUT)
			job->append = 1;
		set_output(job, current->next->token);
	}
}

void	apply_redir(t_token *current, t_jobs *job, t_env *env)
{
	char	*temp;

	if (current->type == HEREDOC)
		apply_heredoc(job, current, env);
	if (current->type == INPUT)
	{
		if (current->next->token && (current->next->token[0] == '$'))
			return (redir_error(job, env, current->next->token,
					REDIR_IN_AMBIGUOUS), (void) NULL);
		temp = unquote_and_direct(current->next->token, env);
		free(current->next->token);
		current->next->token = temp;
		if (access(current->next->token, F_OK) != 0)
			redir_error(job, env, current->next->token, REDIR_IN_NO_FILE);
		else
			set_input(job, current->next->token);
	}
	if (current->type == OUTPUT || current->type == APPEND_OUT)
		apply_output(job, current, env);
}

// void	redir_error(t_jobs *job, bool in_not_out,
//t_env *env, char *str, int *type)
// {
// 	if (env->status != 1)
// 	{
// 		if (type == 0)
// 			ft_printf_fd(2, "minishell: %s: ambiguous redirect\n", str);
// 		else if (type == 1)
// 			ft_printf_fd(2, "minishell: %s: No such file or directory\n", str);
// 	}
// 	env->status = 1;
// 	if (in_not_out)
// 		job->input = ft_strdup("/dev/null");
// 	else
// 		job->output = ft_strdup("/dev/null");
// }

// void	apply_output(t_jobs *job, t_token *current, t_env *env)
// {
// 	char	*temp;
// 	int		fd;

// 	if (current->next->token && (current->next->token[0] == '$'))
// 		return (redir_error(job, false, env,
// current->next->token, 0), (void)NULL);
// 	temp = unquote_and_direct(current->next->token, env);
// 	free(current->next->token);
// 	current->next->token = temp;
// 	fd = open(current->next->token, O_CREAT | O_RDWR, 0644);
// 	if (fd > 0)
// 		close(fd);
// 	if (access(current->next->token, F_OK) != 0)
// 		redir_error(job, false, env, current->next->token, 1);
// 	else
// 	{
// 		if (current->type == APPEND_OUT)
// 			job->append = 1;
// 		set_output(job, current->next->token);
// 	}
// }

// void	apply_redir(t_token *current, t_jobs *job, t_env *env)
// {
// 	int		fd;
// 	char	*temp;

// 	if (current->type == HEREDOC)
// 		apply_heredoc(job, current, env);
// 	if (current->type == INPUT)
// 	{
// 		if (current->next->token && (current->next->token[0] == '$'))
// 			return (redir_error (job, true, env, current->next->token, 0),
// 				(void) NULL);
// 		temp = unquote_and_direct (current->next->token, env);
// 		free (current->next->token);
// 		current->next->token = temp;
// 		if (access(current->next->token, F_OK) != 0)
// 			redir_error(job, true, env, current->next->token, 1);
// 		else
// 			set_input(job, current->next->token);
// 	}
// 	if (current->type == OUTPUT || current->type == APPEND_OUT)
// 		apply_output(job, current, env);
// }
