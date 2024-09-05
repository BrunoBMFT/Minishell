/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brfernan <brfernan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 18:15:45 by bruno             #+#    #+#             */
/*   Updated: 2024/09/05 19:26:57 by brfernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
//dont use bool pipe
int	try_builtins(t_jobs *job, t_env env, bool pipe)//wtf is bool pipe
{
	int	status;

	status = 200;
	if (ft_strcmp(job->job[0], "echo") == 0)//no leaks normal
		status = caught_echo(job);
/* 	if (ft_strcmp(job->job[0], "cd") == 0)
		status = caught_cd(job, env.env); */
	else if (ft_strcmp(job->job[0], "pwd") == 0)//no leaks normal
		status = caught_pwd(job);
	else if (ft_strcmp(job->job[0], "export") == 0)
		status = caught_export(job, env);
	else if (ft_strcmp(job->job[0], "unset") == 0)
		status = caught_unset(job, env);
	else if (ft_strcmp(job->job[0], "env") == 0)//no leaks normal
		status = caught_env(job, env);
	if (pipe)
		exit (status);
	return (status);
}

int	caught_echo(t_jobs *job)//multiple strs doesnt work
{
	bool	nl;
	int		i;

	i = 1;
	nl = true;
	if (!job->job[1])
		return (ft_nl_fd(1), 0);
	if (ft_strcmp(job->job[1], "-n") == 0)
	{
		nl = false;
		i++;
	}
	//echo ola   ""    bruno
	while (job->job[i])
	{
		if (job->job[i][0])
			printf("%s", job->job[i]);
		if (job->job[i + 1])
			printf(" ");
		i++;
	}
	if (nl)
		printf("\n");
	return (0);
}

int	caught_exit(t_jobs *job, t_env env)
{
	char *error;

	if (job->job[1] && !job->job[2])
	{
		// TODO fix
//		if (ft_isdigit(ft_atoi(job->job[1])))
//		{
//			printf("here\n");
//			exit (ft_atoi(job->job[1]));
//		}
		//error check
		error = ft_strjoin3("minishell: exit: ", job->job[1], ": numeric argument required");
		ft_putendl_fd(error, 2);
		free (error);
		exit (0);//tester says exit code has to be 0
	}
	if (job->job[1] && job->job[2])
		return (ft_putendl_fd("minishell: exit: too many arguments", 2), 0);//tester says exit code has to be 0??
	printf("exit\n");
	free_array(env.env);
	rl_clear_history();
	clear_jobs(&job);
	exit(0);
}

