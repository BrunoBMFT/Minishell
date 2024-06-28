/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 12:42:28 by yohan             #+#    #+#             */
/*   Updated: 2024/04/24 18:23:03 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nitems, size_t size)
{
	void	*dest;

	dest = (void *)malloc(nitems * size);
	if (!dest)
		return (NULL);
	ft_bzero(dest, (nitems * size));
	return (dest);
}
/*
char	*ft_return_str(char *str)
{
	int		i;
	char	*dest;

	i = 0;
	dest = (char *)ft_calloc(5, sizeof (char));
	if (dest == NULL)
		return(NULL);
	while (str[i])
	{
		dest[i] = str[i];
		i++;
	}
	return(str);
}

int main (void)
{
	printf("%s", ft_return_str("hello"));
	return (0);
}
*/