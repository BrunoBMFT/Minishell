/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 04:38:31 by bruno             #+#    #+#             */
/*   Updated: 2024/10/16 12:51:38 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	valid_flag(char *str) //fix the return so that as soon as it finds a wrong chaar, it stops
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
					return (0); //flag not valid, print as if not a flag
				i++;
			}
			return (1); //flag is valid, skip to next
		}
	}
	return (0); //argument is not a flag,  must be printed
}

int	caught_echo(t_jobs *job)
{
	bool	nl;
	int		i;
	int 	validFlag;

	i = 0;
	if (!job->job[1])
		return (ft_nl_fd(1), 0);
	validFlag = valid_flag(job->job[1]);
	if (!validFlag)
		i++;
	while (job->job[i])
	{
		if (job->job[i][0])
			ft_printf("%s", job->job[i]);
		if (job->job[i + 1])
			ft_printf(" ");
		i++;
	}
	if (!validFlag)
		ft_printf("\n");
	return (0);
}
