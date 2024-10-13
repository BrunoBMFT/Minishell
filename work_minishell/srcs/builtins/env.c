/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 23:09:17 by bruno             #+#    #+#             */
/*   Updated: 2024/10/13 16:20:52 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	caught_env(t_jobs *job, t_env *env)
{
	int		i;

	if (job->job[1])
	{
		ft_printf_fd(2, "env: '%s': Permission denied\n", job->job[1]);
		return (126);
	}
	i = 0;
	while (env->env[i])
	{
		ft_printf("%s\n", env->env[i]);
		i++;
	}
	return (0);
}
