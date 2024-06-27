/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 22:32:33 by bruno             #+#    #+#             */
/*   Updated: 2024/06/17 20:21:51 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *str, char const *set)
{
	char	*ret;
	int		i;
	int		j;

	if (!str)
		return (NULL);
	ret = 0;
	if (str && set)
	{
		i = 0;
		j = ft_strlen(str);
		while (str[i] && ft_strchr(set, str[i]))
			i++;
		while (str[j - 1] && ft_strchr(set, str[j - 1]) && j > i)
			j--;
		ret = (char *)malloc(sizeof(char) * (j - i + 1));
		if (ret == NULL)
			return (NULL);
		if (ret)
			ft_strlcpy(ret, &str[i], j - i + 1);
	}
	return (ret);
}
