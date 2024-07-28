/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 17:26:33 by bruno             #+#    #+#             */
/*   Updated: 2024/07/25 00:32:51 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	parse_redirections(t_token **token, t_token *cur, int flag)
{
	if (!cur->next)
	{
		ft_printf("Minishell: syntax error near unexpected token`\\n\'\n");
		clear_list(token);
		return (flag + 1);
	}
	else if (cur->next && cur->next->type != WORD)
	{
		ft_printf("Minishell: syntax error near unexpected token `%s\'\n", cur->next->token);
		clear_list(token);
		return (flag + 1);
	}
	return (flag);
}

int	parse_separators(t_token **token, t_token *cur, int flag)
{
	if (cur->type == PIPE && !cur->next)
	{
		ft_printf("work in progress\n");
		clear_list(token);
		return (flag + 1);
	}
	else if (cur->next && cur->next->type >= PIPE && cur->next->type <= OR)
	{
		ft_printf("Minishell: syntax error near unexpected token `%s\'\n", 
			cur->next->token);
		clear_list(token);
		return (flag + 1);
	}
	return (flag);
}

void	parse(t_token **token)
{
	t_token	*cur;
	int		flag;

	cur = *token;
	flag = 0;
	while (cur)
	{ 
		if (cur->type >= INPUT && cur->type <= APPEND_OUT)
			if (flag = parse_redirections (token, cur, flag) != 0)
				return ;
		if (cur && cur->type >= PIPE && cur->type <= OR)
			if (flag = parse_separators (token, cur, flag) != 0)
				return ;
		cur = cur->next;
	}
}
