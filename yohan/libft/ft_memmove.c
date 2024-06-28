/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 12:42:47 by yohan             #+#    #+#             */
/*   Updated: 2024/04/24 18:23:46 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*dest1;
	const unsigned char	*src1;

	dest1 = (unsigned char *)dest;
	src1 = (unsigned const char *)src;
	if (dest == src || n == 0)
		return (dest);
	if (dest > src)
	{
		while (n--)
			dest1[n] = src1[n];
	}
	else
		ft_memcpy(dest, src, n);
	return (dest);
}

// int main (void)
// {
// 	char	*dest;
// 	char *str = "hello000i";

// 	dest = malloc (sizeof (char) * 11);
//     printf("original: %s\n", str);
//     printf("moved: %s\n", (char *)ft_memmove(dest, str, 30));
//     printf("%s", (char *)memmove(dest, src, ));
//     return (0);
// }
