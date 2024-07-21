/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 18:16:48 by bruno             #+#    #+#             */
/*   Updated: 2024/07/21 16:08:41 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

size_t	ft_strlen(const char *str)
{
	size_t	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
		i++;
	return (i);
}
void	ft_bzero(void *str, size_t nb)
{
	size_t			i;
	unsigned char	*ptr;

	i = 0;
	ptr = (unsigned char *)str;
	while (i < nb)
	{
		ptr[i] = 0;
		i++;
	}
}
void	*ft_calloc(size_t nitems, size_t size)
{
	void	*dest;

	dest = (void *)malloc(nitems * size);
	if (!dest)
		return (NULL);
	ft_bzero(dest, (nitems * size));
	return (dest);
}
char	*ft_strndup(char const *s1, int n)// ! TEST
{
	int		i;
	char	*dest;

	if (!s1)
		return (NULL);
	dest = (char *)malloc(sizeof(char) * ft_strlen(s1) + 1);
	if (!dest)
		return (NULL);
	i = 0;
	while (s1[i] && i < n)
	{
		dest[i] = s1[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

int	ft_strncmp(const char *s1, char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while (s1[i] && s1[i] == s2[i] && i < (n - 1))
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}



int	len_to_equal(char *str)
{
	int i;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	return (i);
}
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
