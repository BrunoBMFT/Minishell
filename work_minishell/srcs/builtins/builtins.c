/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 18:15:45 by bruno             #+#    #+#             */
/*   Updated: 2024/09/07 04:51:36 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
//dont use bool pipe

void	process_exit(t_jobs *job, t_env env, int status)
{
	free_array(env.env);
	rl_clear_history();
	clear_jobs(&job);
	exit(status);
}
//exit goes here?
int	try_builtins(t_jobs *job, t_env env, bool pipe)//wtf is bool pipe
{
	int	status;

	status = 200;
	if (ft_strcmp(job->job[0], "echo") == 0)
		status = caught_echo(job);
	else if (ft_strcmp(job->job[0], "pwd") == 0)
		status = caught_pwd(job);
	else if (ft_strcmp(job->job[0], "export") == 0)
		status = caught_export(job, env);
	else if (ft_strcmp(job->job[0], "unset") == 0)
		status = caught_unset(job, env);
	else if (ft_strcmp(job->job[0], "env") == 0)
		status = caught_env(job, env);
	if (pipe)
		process_exit(job, env, status);//use some other function
	return (status);
}

int	caught_exit(t_jobs *job, t_env env)//send
{
	if (job->job[1])
	{
		if (job->job[2])
			return (ft_printf_fd(2, "minishell: exit: too many arguments\n"), 2);
		if (ft_isdigit(ft_atoi(job->job[1])))
		{
			printf("exit here\n");
			clean_exit(job, env);
			exit (2);//error message
		}
		ft_printf_fd(2, "minishell: exit: %s: numeric argument required\n", job->job[1]);
		clean_exit(job, env);
		exit (2);
	}
	printf("exit\n");
	free_array(env.env);
	rl_clear_history();
	clear_jobs(&job);
	exit(0);
}

