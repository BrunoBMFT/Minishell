/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 21:30:07 by bruno             #+#    #+#             */
/*   Updated: 2024/06/28 12:22:14 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	caught_exit_status()
{
	ft_putendl_fd("nice exit code expanded here", 1);//stdout?
}


bool	parse_quotation_mark(char *input)
{
	int	closed1;
	int	closed2;

	closed1 = 0;
	closed2 = 0;
	int	i = 0, j = 0;
	while (input[i])
	{
		if (input[i] == 34)// int for "
			closed1++;
		else if (input[i] == 39)// int for '
			closed2++;
		else
		{
			input[j] = input[i];
			j++;
		}
		i++;
	}
	i = 0;
	while (i < closed1 || i < closed2)
	{
		input[j] = 0;
		i++;
	}
	if (ft_is_even(closed1) && ft_is_even(closed2))
		return (true);
	return (false);
}
