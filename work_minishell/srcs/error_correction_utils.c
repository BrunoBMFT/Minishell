/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_correction_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 03:18:24 by bruno             #+#    #+#             */
/*   Updated: 2024/08/29 22:24:29 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	count_quotes(char *str, int *i)
{
	char	quote;
	int		count;

	count = 1;
	quote = str[(*i)++];
	while (str[*i] && str[*i] != quote)
	{
		count++;
		(*i)++;
	}
	if (str[*i] == quote)
	{
		count++;
		(*i)++;
	}
	return (count);
}

int	count_special_chars(char *str, int *i)
{
	int	count;

	count = 0;
	while (str[*i] && (str[*i] == '&' || str[*i] == '|'
			|| str[*i] == '<' || str[*i] == '>'))
	{
		count++;
		(*i)++;
	}
	return (count + 1);
}

int	count_normal_chars(char *str, int *i)
{
	int	count;

	count = 0;
	while (str[*i] && str[*i] != '&' && str[*i] != '|'
		&& str[*i] != '<' && str[*i] != '>'
		&& str[*i] != '\t' && str[*i] != ' ')
	{
		count++;
		(*i)++;
	}
	return (count + 1);
}
