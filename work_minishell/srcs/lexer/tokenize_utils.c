/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 16:52:00 by ycantin           #+#    #+#             */
/*   Updated: 2024/10/30 15:01:56 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*addtok(void *content)
{
	t_token	*list;

	list = malloc(sizeof(t_token));
	if (!list)
		return (NULL);
	list->token = content;
	list->next = NULL;
	return (list);
}

t_token	*get_last_tok(t_token *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	go_to_next(t_token **lst, t_token *new)
{
	t_token	*current;

	if (!new)
		return ;
	current = get_last_tok(*lst);
	if (lst && current)
		current->next = new;
	else
		*lst = new;
}

int	define_type(char *str)
{
	int	type;

	if (strcmp(str, "||") == 0)
		type = OR;
	else if (ft_strcmp(str, "&&") == 0)
		type = AND;
	else if (ft_strcmp(str, "|") == 0)
		type = PIPE;
	else if (ft_strcmp(str, "<") == 0)
		type = INPUT;
	else if (ft_strcmp(str, ">") == 0)
		type = OUTPUT;
	else if (ft_strcmp(str, "<<") == 0)
		type = HEREDOC;
	else if (ft_strcmp(str, ">>") == 0)
		type = APPEND_OUT;
	else
		type = WORD;
	return (type);
}

char	**modify_array(char **array, t_env *env)
{
	int		i;
	char	*temp;
	char	**new;
	int		j = 0;

	i = 0;
	int k = 0;
	while (array[i])
	{
		temp = unquote_and_direct(array[i], env);
		if (temp && temp[0])
			k++;
		free(array[i]);//losing
		array[i] = temp;
		i++;
	}
	array[i] = NULL;
	new = ft_calloc(sizeof(char *), k + 1);
	if (!new)
		return (array);
	i = 0;
	while (array[i])
	{
		if (array[i][0])
		{
			new[j] = ft_strdup(array[i]);
			j++;
		}
		i++;
	}
	free_array(array);
	new[j] = NULL;
	return (new);
}


