/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 17:26:33 by bruno             #+#    #+#             */
/*   Updated: 2024/11/08 05:38:05 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	check_separator_syntax(char *str, char separator, int dir)
{
	char	*newline;
	char	*unexpected;

	newline = "minishell: syntax error near unexpected token `newline'\n";
	unexpected = "minishell: syntax error near unexpected token `";
	if (!dir)
	{
		if (*(str + 1) == separator && !*(str + 2))
			ft_printf_fd(2, newline);
		else if (!*(str + 2))
			ft_printf_fd(2, newline);
	}
	else
	{
		if (*(str + 1) && *(str + 1) == separator)
			ft_printf_fd(2, "%s%c%c\'\n", unexpected, separator, separator);
		else
			ft_printf_fd(2, "%s%c\'\n", unexpected, separator);
	}
}

int	syntax_check_wrapper(char *str, char target)
{
	if (*(str + 1) == '>')
		return (check_separator_syntax(str, '>', 0), -1);
	else if (*(str + 1) == '<')
		return (check_separator_syntax(str, '<', 0), -1);
	else
		return (check_separator_syntax(str, target, 1), -1);
	return (0);
}

int	parse_string(char *str, t_var_holder *h)
{
	if (*str && *str == h->b)
		return (syntax_check_wrapper(str, h->b));
	else if (*str && *str == h->c)
		return (syntax_check_wrapper(str, h->c));
	else if (*str && *str == h->d)
		return (syntax_check_wrapper(str, h->d));
	return (0);
}

int	start_string_parse(char *str, char delimiter, t_var_holder *h)
{
	if (delimiter == '&')
	{
		h->b = '|';
		h->c = '<';
		h->d = '>';
	}
	else if (delimiter == '|')
	{
		h->b = '&';
		h->c = '<';
		h->d = '>';
	}
	else if (delimiter == '<')
	{
		h->b = '&';
		h->c = '|';
		h->d = '>';
	}
	else if (delimiter == '>')
	{
		h->b = '&';
		h->c = '|';
		h->d = '<';
	}
	return (parse_string(str, h));
}

int	parse_token(t_token *t, bool *in_sq, bool *in_dq, t_var_holder *h)
{
	t_var_holder	i;

	i.temp = "minishell: syntax error near unexpected token `newline'\n";
	i.temp2 = "minishell: syntax error near unexpected token `";
	i.i = 0;
	while (t->token[i.i])
	{
		if (t->token[i.i] == '\'' && !*in_dq)
			*in_sq = !*in_sq;
		else if (t->token[i.i] == '\"' && !*in_sq)
			*in_dq = !*in_dq;
		if (*in_sq || *in_dq)
		{
			i.i++;
			continue ;
		}
		if (t->token[i.i] == '&' || t->token[i.i] == '|')
		{
			if (start_string_parse(t->token + (i.i + 1)
					, t->token[i.i], h) == -1)
				return (-1);
		}
		else if (t->token[i.i] == '<' || t->token[i.i] == '>')
		{
			i.j = i.i;
			i.count = 0;
			while (t->token[i.j] && t->token[i.j] == '>'
				|| t->token[i.j] == '<')
			{
				i.j++;
				i.count++;
			}
			if (i.count > 2)
				return (ft_printf_fd(2, "%s%c\n", i.temp2
						, t->token[i.count - 1]), -1);
			i.len = ft_strlen(t->token) - 1;
			if (i.len != 1 && (t->token[i.len] == '<'
					|| t->token[i.len] == '>') && !t->next)
				return (ft_printf_fd(2, "%s", i.temp), -1);
			if (start_string_parse((t->token + (i.i + 1))
					, t->token[i.i], h) == -1)
				return (-1);
		}
		i.i++;
	}
	return (0);
}
//handle case like echo<> or echo<"THERE"< as errors while still 
//allowing echo<"THERE" -----> I might have to check my error 
//correction function so that it splits when finding redirections 
