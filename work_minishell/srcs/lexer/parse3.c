/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 17:26:33 by bruno             #+#    #+#             */
/*   Updated: 2024/11/22 07:24:24 by ycantin          ###   ########.fr       */
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

int	validate_redirection_syntax(t_token *t, t_var_holder *i)
{
	const char	*syntax_error;
	const char	*unexpected_token;

	unexpected_token = "minishell: syntax error near unexpected token `";
	syntax_error = "minishell: syntax error near unexpected token `newline'\n";
	i->j = i->i;
	i->count = 0;
	while (t->token[i->j] && (t->token[i->j] == '>' || t->token[i->j] == '<'))
	{
		i->j++;
		i->count++;
	}
	if (i->count > 2)
		return (ft_printf_fd(2, "%s%c\n"
				, unexpected_token, t->token[i->count - 1]), -1);
	if ((ft_strlen(t->token) - 1 == i->i) && !t->next)
		return (ft_printf_fd(2, "%s", syntax_error), -1);
	return (0);
}

int	handle_special_characters(t_token *t, t_var_holder *h, t_var_holder *i)
{
	if (t->token[i->i] == '&' || t->token[i->i] == '|')
	{
		if (start_string_parse(t->token + (i->i + 1), t->token[i->i], h) == -1)
			return (-1);
	}
	else if (t->token[i->i] == '<' || t->token[i->i] == '>')
	{
		if (validate_redirection_syntax(t, i) == -1)
			return (-1);
		if (start_string_parse(t->token + (i->i + 1), t->token[i->i], h) == -1)
			return (-1);
	}
	return (0);
}

void	toggle_quote_state(char c, bool *in_sq, bool *in_dq)
{
	if (c == '\'' && !*in_dq)
		*in_sq = !*in_sq;
	else if (c == '\"' && !*in_sq)
		*in_dq = !*in_dq;
}

int	parse_token(t_token *t, bool *in_sq, bool *in_dq, t_var_holder *h)
{
	t_var_holder	i;

	i.i = 0;
	while (t->token[i.i])
	{
		toggle_quote_state(t->token[i.i], in_sq, in_dq);
		if (*in_sq || *in_dq)
		{
			i.i++;
			continue ;
		}
		if (handle_special_characters(t, h, &i) == -1)
			return (-1);
		i.i++;
	}
	return (0);
}
