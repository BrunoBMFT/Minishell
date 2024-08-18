
#include "includes/minishell.h"

char	*ft_strndup(char const *s1, int n)
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
int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
		i++;
	return (s1[i] - s2[i]);
}
char	*ft_strdup(const char *s1)
{
	int		i;
	char	*dest;

	i = 0;
	while (s1[i])
		i++;
	dest = (char *)malloc((i + 1) * sizeof(char));
	if (dest == NULL)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		dest[i] = s1[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
	free(dest);
}
int	len_to_equal(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	return (i);
}
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
char	*ft_strnstr(const char *hay, const char *needle, size_t n)
{
	size_t	i;
	size_t	j;
	int		needlelen;

	i = 0;
	needlelen = ft_strlen(needle);
	if (needlelen == 0)
		return ((char *)hay);
	while (i < n && hay[i])
	{
		j = 0;
		while (needle[j] == hay[i + j] && (i + j) < n)
		{
			if (needle[j + 1] == '\0')
				return ((char *)hay + i);
			j++;
		}
		i++;
	}
	return (0);
}

bool	is_in_env(char *to_add, char ***env)
{
	int	i;
	char	*temp1 = NULL;
	char	*temp2 = NULL;

	if (!to_add || !env)
		return false;
	i = 0;
	while (*env[i])
	{
		temp1 = ft_strndup(to_add, len_to_equal(to_add));
		temp2 = ft_strndup(*env[i], len_to_equal(*env[i]));
		if (ft_strcmp(temp1, temp2) == 0)
		{
			*env[i] = ft_strdup(to_add);
			return (true);
		}
		i++;
	}
	return (false);
}

int	main(int ac, char **av, char **envp)
{
	if (is_in_env("USER=CHANGED_USER", &envp))
		printf("it is in env\n");
	else
		printf("not in\n");
	int i = 0;
	while (envp[i])
	{
		if (ft_strcmp("USER=CHANGED_USER", envp[i]) == 0)
			printf("changed %s\n", envp[i]);
		i++;
	}
}