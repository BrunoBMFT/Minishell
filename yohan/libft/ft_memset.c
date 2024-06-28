/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 12:42:51 by yohan             #+#    #+#             */
/*   Updated: 2024/04/24 18:23:49 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *str, int c, size_t n)
{
	size_t			i;
	unsigned char	*ptr;

	i = 0;
	ptr = (unsigned char *)str;
	while (i < n)
	{
		ptr[i] = c;
		i++;
	}
	return (ptr);
}

// int main (void)
// {
// 	char	*str;

// 	str = ft_strdup("hello worlds");
// 	if (str == NULL)
// 		return (1);
//     printf("mine: %s\n", (char *) ft_memset(str, '1', sizeof (char) * 4));
//     //printf("real: %s", (char *) memset(str, 49, sizeof (char) * 4));
// 	free (str);
//     return (0);
// }
