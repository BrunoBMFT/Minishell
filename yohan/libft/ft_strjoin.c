/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 12:43:06 by yohan             #+#    #+#             */
/*   Updated: 2024/04/24 18:24:18 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		i;
	int		j;
	char	*full;

	j = 0;
	i = 0;
	full = malloc((ft_strlen(s1) + ft_strlen(s2)) + 1 * sizeof(char));
	if (full == NULL)
		return (NULL);
	while (s1[i])
	{
		full[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		full[i] = s2[j];
		i++;
		j++;
	}
	full[i] = '\0';
	return (full);
}

/*
int main (int argc, char*argv[])
{
	if (argc != 3)
		return (0);
	else
	{
		printf("%s", ft_strjoin(argv[1], argv[2]));
		return (0);
	}
}
 */