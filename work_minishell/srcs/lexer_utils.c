/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 16:52:00 by ycantin           #+#    #+#             */
/*   Updated: 2024/07/10 22:06:22 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
	int type;

	if (strncmp(str, "||", 2) == 0)
		type = TOKEN_OR;
	else if (ft_strncmp(str, "&&", 2) == 0)
		type = TOKEN_AND;
	else if (ft_strncmp(str, "|", 1) == 0)
		type = TOKEN_PIPE;
	else if(ft_strncmp(str, "<", 1) == 0)
		type = TOKEN_REDIR_IN;
	else if(ft_strncmp(str, ">", 1) == 0)
		type = TOKEN_REDIR_OUT;
	else if(ft_strncmp(str, "<<", 2) == 0)
		type = TOKEN_REDIR_HEREDOC;
	else if(ft_strncmp(str, ">>", 2) == 0)
		type = TOKEN_REDIR_APPEND_OUT;
	else
		type = TOKEN_WORD;
	return (type);
}
