/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 14:46:14 by bruno             #+#    #+#             */
/*   Updated: 2024/11/22 09:59:32 by ycantin          ###   ########.fr       */
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
