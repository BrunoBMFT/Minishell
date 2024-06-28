/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 12:41:16 by yohan             #+#    #+#             */
/*   Updated: 2024/04/24 18:23:13 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isdigit(int c)
{
	if (c >= 48 && c <= 57)
		return (1);
	return (0);
}
/*
int main (void)
{
	int		i;
	char	x[];

	i = 0;
	x[] = "12ndie345a";
	while (x[i])
	{
		printf("%d", ft_isdigit(x[i]));
		i++;
	}
		return (0);
}
*/