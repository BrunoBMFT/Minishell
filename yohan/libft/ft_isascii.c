/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 12:42:18 by yohan             #+#    #+#             */
/*   Updated: 2024/04/24 18:23:11 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isascii(int input)
{
	if (input >= 0 && input <= 127)
		return (1);
	return (0);
}
/*
int main (int argc, char *argv[])
{
	int	i;

	i = 0;
	(void) argc;
	while (argv[1][i])
	{
		printf ("%d", ft_isascii(argv[1][i]));
		i++;
	}
	return (0);
}
*/