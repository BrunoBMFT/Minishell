/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 18:16:48 by bruno             #+#    #+#             */
/*   Updated: 2024/07/19 23:44:31 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_env_var(char *str)//wrong size allocated
{
	int		i;
	int		j;
	char	*new;

	i = 0;
	j = 0;
	while (str[i] && str[i] != '=')
		i++;
	i++;
	new = ft_calloc(sizeof(char), (ft_strlen(str) - len_to_equal(str)));
	if (!new)
		return (NULL);
	while (str[i])
	{
		new[j] = str[i];
		j++;
		i++;
	}
	return (new);
}

int	len_to_equal(char *str)
{
	int i;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	return (i);
}

char	*ft_getenv(char *str, char **env)
{
	char	*temp;
	int		i;

	i = 0;
	while (env[i])
	{
		temp = ft_strndup(env[i], len_to_equal(env[i]));
		if (ft_strncmp(str, temp, ft_strlen(temp)) == 0)
			return (ft_env_var(env[i]));
		i++;
	}
	return (NULL);
}