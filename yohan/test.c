
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// typedef struct s_andor
// {
//     char *token;
//     char *and_or;
//     char *job;
//     //t_andor *next;
// }   t_andor;

// size_t	ft_strlen(const char *str)
// {
// 	size_t	i;

// 	i = 0;
// 	while (str[i])
// 		i++;
// 	return (i);
// }

// char	*ft_strrchr(const char *str, int c)
// {
// 	int		i;
// 	char	*s;

// 	i = ft_strlen(str);
// 	s = (char *)str;
// 	if (c == 0)
// 		return (s + i);
// 	while (i >= 0)
// 	{
// 		if (str[i] == (char)c)
// 			return (s + i);
// 		i--;
// 	}
// 	return (NULL);
// }

// char	*ft_strchr(const char *s, int c)
// {
// 	int	i;

// 	i = 0;
// 	while (s[i] != (char)c && s[i] != '\0')
// 		s++;
// 	if (s[i] == (char)c)
// 		return ((char *)s);
// 	return (0);
// }



// void    get_token(t_andor *andor, char *str, int i)
// {
//     if (str[i] && str[i] == '&' && str[i + 1] == '&')
//         andor->token = "&&";
//     else if (str[i] && str[i] == '|' && str[i + 1] == '|')
//         andor->token = "||";
//     else
//         andor->token = NULL;
// }



// char *token_split(t_andor *andor, char *str)
// {
//     int i;
//     int j;
//     char *temp;

//     i = 0;
//     j = 0;
//     while (str[j] && str[j] != '&' && str[j] + 1 != '&')
//         j++;
//     get_token(andor, str, j);
//     temp = malloc (sizeof (char) * j + 1);
//     if (!temp)
//         return(NULL);
//     while (i < j)
//     {
//         temp[i] = str[i];
//         i++;
//     }
//     temp[i] = '\0';
//     andor->job = temp;
//     return(&str[i + 2]);
// }

// int main (void)
// {
//     t_andor andor;
//     char *str = "cat makefile | grep a && help || cat e";
//     printf ("and/or:  %s\n", token_split(&andor, str));
//     printf  ("token:    %s\n", andor.token);
//     printf  ("job:      %s", andor.job);
//     return (0);
// }