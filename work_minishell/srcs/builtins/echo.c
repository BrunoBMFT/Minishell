/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 04:38:31 by bruno             #+#    #+#             */
/*   Updated: 2024/10/28 18:57:58 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	n_flag(char *str) //fix the return so that as soon as it finds a wrong chaar, it stops
{
	int i;

	i = 0;
	while (str[i] == ' ' && str[i] == '\t' && str[i] == '\n')
		i++;
	if (str[i] == '-') //arg is flag
	{
		i++;
		if (str[i] == 'n')
		{
			while (str[i] && (str[i] != ' ' && str[i] != '\t' && str[i] != '\n'))
			{	
				if (str[i] != 'n')
					return (false); //flag not valid, print as if not a flag
				i++;
			}
			return (true); //flag is valid, skip to next
		}
	}
	return (false); //argument is not a flag,  must be printed
}

int	caught_echo(t_jobs *job)
{
	int		i;
	bool 	has_n_flag;

	i = 1;
	if (!job->job[1])
		return (ft_nl_fd(1), 0);
	has_n_flag = n_flag(job->job[1]);
	if (has_n_flag)
		i++;
	while (job->job[i])
	{
		if (job->job[i][0])
			ft_printf_fd(1, "%s", job->job[i]);//use putstr instead?
		if (job->job[i + 1])
			ft_printf_fd(1, " ");
		i++;
	}
	if (!has_n_flag)
		ft_printf("\n");
	return (0);
}
