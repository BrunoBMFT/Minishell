/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:20:43 by ycantin           #+#    #+#             */
/*   Updated: 2024/08/16 01:00:53 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	tokenize(t_token **list, char *str)
{
	int		i;
	char	**array;
	t_token	*new_node;

	i = 0;
	array = token_array(str);
	if (!array)
		return;
	while (array[i])
	{
		new_node = addtok(ft_strdup(array[i]));
		if (!new_node)
			free_all(list, array, "Error\n", 7);
		new_node->type = define_type(array[i]);
		go_to_next(list, new_node);
		i++;
	}
	free_array(array);
}

int	count_words(char *str)
{
	t_var_holder	h;

	h.i = 0;
	h.wc = 0;
	h.k = 0;
	if (!str)
		return (0);
	while (str[h.i])
	{
		if (!h.k && !(str[h.i] == ' ' || str[h.i] == '\t' || str[h.i] == '\n'))
		{
			h.wc++;
			h.k = 1;
		}
		if (str[h.i] == ' ' || str[h.i] == '\t' || str[h.i] == '\n')
			h.k = 0;
		h.i++;
	}
	return (h.wc);
}

char	**token_array(char *str)
{
	t_var_holder	h;

	h.i = 0;
	h.j = 0;
	h.k = 0;
	h.wc = count_words(str);
	h.array = malloc(sizeof(char *) * (h.wc + 1));
	if (!h.array)
		return (NULL);
	while (str[h.i])
	{
		while (str[h.i] && (str[h.i] == ' ' || str[h.i] == '\t' || str[h.i] == '\n'))
			h.i++;
		h.j = h.i;
		while (str[h.i] && !(str[h.i] == ' ' || str[h.i] == '\t' || str[h.i] == '\n'))
			h.i++;
		if (h.i > h.j)
		{
			h.array[h.k] = ft_substr(str, h.j, h.i - h.j);
			h.k++;
		}
	}
	h.array[h.k] = NULL;
	return (h.array);
}
