/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brfernan <brfernan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 21:01:01 by bruno             #+#    #+#             */
/*   Updated: 2024/10/14 14:59:14 by brfernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/minishell.h"

void	clean_exit(t_jobs *jobs, t_env *env, int status)
{
	clear_jobs(&jobs);//if
	close (env->saved_stdin);//if
	close (env->saved_stdout);//if
	free_array(env->env);//if
	rl_clear_history();
	exit (status);
}

bool	parse_digit(char *str)
{
	int	i;

	i = 0;
	if (str[0] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if ((str[i] < '0' || str[i] > '9') || str[i] == '-' || str[i] == '+')
			return (false);
		i++;
	}
	return (true);
}

int	caught_exit(t_jobs *job, t_env *env, bool piped)
{
	if (!piped)
		printf("exit\n");
	if (job->job[1])
	{
		if (!parse_digit(job->job[1]))
		{
			ft_printf_fd(2, "minishell: exit: %s: numeric argument required\n", job->job[1]);
			clean_exit(job, env, 2);
		}
		if (job->job[2])
			return (ft_printf_fd(2, "minishell: exit: too many arguments\n"), 1);
	}
	if (!piped)
	{
		if (job->job[1])
			clean_exit(job, env, ft_atoi(job->job[1]));
		clean_exit(job, env, env->status);
	}
	return (0);
}
