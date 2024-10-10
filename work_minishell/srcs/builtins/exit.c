/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brfernan <brfernan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 21:01:01 by bruno             #+#    #+#             */
/*   Updated: 2024/10/10 14:18:47 by brfernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/minishell.h"

//clear rl history
void	clean_exit(t_jobs *jobs, t_env *env, int status)
{
	clear_jobs(&jobs);
	close (env->saved_stdin);
	close (env->saved_stdout);
	free_array(env->env);
	exit (status);
}

bool	parse_digit(char *str)
{
	int	i;

	i = 0;
	if (str[0] == '-')
		i++;
	while (str[i])
	{
		if ((str[i] < '0' || str[i] > '9') || str[i] == '-')
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
			rl_clear_history();//put it in clean exit
			clean_exit(job, env, 2);
		}
		if (job->job[2])
			return (ft_printf_fd(2, "minishell: exit: too many arguments\n"), 1);
	}
	if (!piped)
	{
		rl_clear_history();//clear history in clean exit
		clean_exit(job, env, 0);
	}
	return (0);
}
