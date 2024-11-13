/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 17:26:33 by bruno             #+#    #+#             */
/*   Updated: 2024/11/08 05:46:16 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	secondquote(char *line)//rather not use
{
	int		i;
	int		error;
	int		inside;
	char	quote;

	i = 0;
	error = 0;
	inside = 0;
	while (line[i])
	{
		if (!inside && (line[i] == '\'' || line[i] == '\"'))
		{
			quote = line[i];
			inside = 1;
			error = 1;
		}
		else if (inside == 1 && line[i] == quote)
		{
			error = 0;
			inside = 0;
		}
		i++;
	}
	return (error);
}

// char	*parse_quotes(char *line)
// {
// 	int		i;
// 	char	*start;
// 	char	*new;
// 	char	*newline;

// 	i = 0;
// 	if (!line)
// 		return (NULL);
// 	while (line[i])
// 	{
// 		if (line[i] == '\'' || line[i] == '\"')
// 		{
// 			start = ft_strchr(line + i + 1, line[i]);
// 			while (!start)
// 			{
// 				newline = readline("quote> ");
// 				if (!newline)
// 					new = ft_strjoin(line, "\n");
// 				else
// 					new = ft_strjoin3(line, "\n", newline);
// 				free(line);
// 				free(newline);
// 				line = new;
// 				start = ft_strchr(line + i + 1, line[i]);
// 			}
// 			i = start - line;
// 		}
// 		i++;
// 	}
// 	return (line);
// }
