/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 23:09:17 by bruno             #+#    #+#             */
/*   Updated: 2024/11/01 02:32:34 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	caught_printenv(t_env *env)//not needed
{
	int		i;

	i = 0;
	while (env->env[i])
	{
		ft_printf("%s\n", env->env[i]);
		i++;
	}
	return (0);
}

int	caught_env(t_jobs *job, t_env *env)
{
	int		i;
	int		j;
	bool	print;

	i = 0;
	if (job->job[1])
		return (ft_printf_fd(2,
				"env: '%s': Permission denied\n", job->job[1]), 126);
	while (env->env[i])
	{
		j = 0;
		print = 0;
		while (env->env[i][j])
		{
			if (env->env[i][j++] == '=')
			{
				print = 1;
				break ;
			}
		}
		if (print)
			ft_printf("%s\n", env->env[i]);
		i++;
	}
	return (0);
}
