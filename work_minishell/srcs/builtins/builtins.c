/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 18:15:45 by bruno             #+#    #+#             */
/*   Updated: 2024/10/03 17:38:12 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
//dont use bool pipe

int	try_builtins(t_jobs *job, t_env env, bool pipe)
{
	int	status;

	status = 200;
	if (ft_strcmp(job->job[0], "echo") == 0)
		status = caught_echo(job);
	else if (ft_strcmp(job->job[0], "pwd") == 0)
		status = caught_pwd(job);
	else if (ft_strcmp(job->job[0], "export") == 0)
		status = caught_export(job, &env);
	else if (ft_strcmp(job->job[0], "unset") == 0)
		status = caught_unset(job, env);//use & as well?
	else if (ft_strcmp(job->job[0], "env") == 0)
		status = caught_env(job, env);
	else if (ft_strcmp(job->job[0], "exit") == 0)
		status = caught_exit(job, env, pipe);
	if (pipe && status != 200)
		clean_exit(job, env, status);//use some other function
	return (status);
}
