
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 17:26:33 by bruno             #+#    #+#             */
/*   Updated: 2024/09/11 11:27:32 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	parse_redirections(t_token **token, t_token *cur, int flag)
{
	if (!cur->next)
	{
		ft_printf_fd(2, "minishell: syntax error near unexpected token`\\n\'\n");
		clear_list(token);
		return (flag + 1);
	}
	else if (cur->next && cur->next->type != WORD)
	{
		ft_printf_fd(2, "minishell: syntax error near unexpected token `%s\'\n", cur->next->token);
		clear_list(token);
		return (flag + 1);
	}
	return (flag);
}

int parse_separators(t_token **token, t_token *cur, int flag)
{
    if (cur->next && cur->next->type >= PIPE && cur->next->type <= OR)
    {
        if (cur->type >= PIPE && cur->type <= OR) // Check for double delimiters
        {
            ft_printf_fd(2, "minishell: syntax error near unexpected token `%s\'\n", cur->next->token);
            clear_list(token);
            return (flag + 1);
        }
    }
    return (flag);
}

void	check_separator_syntax(char *str, char separator, int dir)
{
	char *newline;
	char *unexpected;

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
	else if(delimiter == '|')
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

int parse_token(t_token *t, bool *in_sq, bool *in_dq, t_var_holder *h)
{
	int i;
	int j;
	int count;

	i = 0;
	while (t->token[i])
	{
		if (t->token[i] == '\'' && !*in_dq)
			*in_sq = !*in_sq;
		else if (t->token[i] == '\"' && !*in_sq)
			*in_dq = !*in_dq;
		if (*in_sq || *in_dq)
		{
			i++;
			continue ;
		}
		if (t->token[i] == '&' || t->token[i] == '|')
		{
			if (start_string_parse(t->token + (i + 1), t->token[i], h) == -1)
				return (-1);
		}
		else if (t->token[i] == '<' || t->token[i] == '>')
		{
			j = i;
			count = 0;
			while (t->token[j] && t->token[j] == '>' || t->token[j] =='<')
			{
				j++;
				count++;
			}
			if (count > 2)
				return (ft_printf_fd(2, "minishell: syntax error near unexpected token `%c\n", t->token[count]), -1);
			int len = ft_strlen(t->token) - 1;

			printf("full:%s\n", t->token);
			printf("here:%s\n", t->token + i);
			printf("length:%d\n", len);
			if (len != 1 && (t->token[len] == '<' || t->token[len] == '>') && !t->next)
			{
				printf("hey\n");
				return (ft_printf_fd(2, "minishell: syntax error near unexpected token `newline'\n"), -1);
			}
			if (start_string_parse((t->token + (i + 1)), t->token[i], h) == -1)
				return (-1);
		}
		i++;
	}
	return (0);
} //handle case like echo<> or echo<"THERE"< as errors while still allowing echo<"THERE" -----> I might have to check my error correction function so that it splits when finding redirections 

int	parse_seps_and_redirs(t_token **token, t_token *cur)
{
	int flag;

	flag = 0;
	while (cur && cur->next)
	{ 
		if (cur->type >= INPUT && cur->type <= APPEND_OUT)
			if ((flag = parse_redirections(token, cur, flag)) != 0)
				return (-1);
		if (cur->type >= PIPE && cur->type <= OR)
			if ((flag = parse_separators(token, cur, flag)) != 0)
				return (-1);
		cur = cur->next;
	}
	return (0);
}

int parse(t_token **token)
{
	t_var_holder h;
	t_token *cur;
	int flag;

	cur = *token;
	flag = 0;
	if (cur && cur->type >= PIPE && cur->type <= OR)
	{
		ft_printf_fd(2, "minishell: syntax error near unexpected token `%s\'\n", cur->token);
		clear_list(token);
		return (-1);
	}
	if (parse_seps_and_redirs(token, cur) == -1)
		return (-1);
	cur = *token;
	while (cur)
	{
		bool in_sq = false;
		bool in_dq = false;
		if (parse_token(cur, &in_sq, &in_dq, &h) == -1)
			return (-1);
		cur = cur->next;
	}
	if (cur && cur->type >= PIPE && cur->type <= OR)
	{
		ft_printf_fd(2, "minishell: syntax error near unexpected token `%s\'\n", cur->token);
		clear_list(token);
		return (-1);
	}
	return (0);
}


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

char	*parse_quotes(char *line)
{
	int		i;
	char	*start;
	char	*new;
	char	*newline;

	i = 0;
	if (!line)
		return (NULL);
	while (line[i])
	{
		if (line[i] == '\'' || line[i] == '\"')
		{
			start = ft_strchr(line + i + 1, line[i]);
			while (!start)
			{
				newline = readline("quote> ");
				if (!newline)
					new = ft_strjoin(line, "\n"); //not working rn
				else
					new = ft_strjoin3(line, "\n", newline);//seems to be working
				free(line);
				free(newline);
				line = new;
				start = ft_strchr(line + i + 1, line[i]);
			}
			i = start - line;
		}
		i++;
	}
	return (line);
}

int	parse_last_token(char **cmd_line, t_token **list, t_token **last)
{
	char	*line;
	char	*new;
	char	*converted;

	line = readline("> ");
	if (!line)
		return (clean_up_build(list, *cmd_line), -1);
	new = ft_strjoin(*cmd_line, line);
	free(*cmd_line);
	free(line);
	*cmd_line = new;
	clear_list(list);
	converted = split_complex_args(*cmd_line);
	tokenize(list, converted);
	free(converted);
	*last = get_last_tok(*list);
	if (parse(list) == -1)
	{
		clean_up_build(list, *cmd_line);
		return (-1);
	}
	return (0);
}
