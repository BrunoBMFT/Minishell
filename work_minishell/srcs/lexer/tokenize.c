/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 01:39:10 by bruno             #+#    #+#             */
/*   Updated: 2024/09/27 01:39:14 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	tokenize(t_token **list, char *str, t_env env)
{
	int		i;
	char	**array;
	char	*temp;
	t_token	*new_node;

	i = 0;
	array = token_array(str);
	//modify_array(array, env, NULL);
	if (!array)
		return ;
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

void	handle_quotes(t_var_holder *h, char *str)
{
	char	quote;

	quote = str[h->i];
	h->i++;
	while (str[h->i] && str[h->i] != quote)
		h->i++;
	if (str[h->i] == quote)
		h->i++;
}

int	count_words(char *str)
{
	t_var_holder	h;

	h.i = 0;
	h.wc = 0;
	while (str[h.i])
	{
		while (str[h.i] && (str[h.i] == ' ' || str[h.i] == '\t'
				|| str[h.i] == '\n'))
			h.i++;
		if (str[h.i] == '\'' || str[h.i] == '\"')
		{
			handle_quotes(&h, str);
			h.wc++;
		}
		else if (str[h.i] && !(str[h.i] == ' ' || str[h.i] == '\t'
				|| str[h.i] == '\n'))
		{
			while (str[h.i] && !(str[h.i] == ' ' || str[h.i] == '\t'
					|| str[h.i] == '\n'))
				h.i++;
			h.wc++;
		}
	}
	return (h.wc);
}

void	update_iterator(t_var_holder *h, char *str)
{
	while (str[h->i])
	{
		while (str[h->i] && (str[h->i] == ' ' || str[h->i] == '\t'
				|| str[h->i] == '\n'))
			h->i++;
		h->j = h->i;
		if (str[h->i] == '\'' || str[h->i] == '\"')
			handle_quotes(h, str);
		else
		{
			while (str[h->i] && !(str[h->i] == ' ' || str[h->i] == '\t'
					|| str[h->i] == '\n'))
				h->i++;
		}
		if (h->i > h->j)
		{
			h->array[h->k] = ft_substr(str, h->j, h->i - h->j);
			h->k++;
		}
	}
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
	update_iterator(&h, str);
	h.array[h.k] = NULL;
	return (h.array);
}