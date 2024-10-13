
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

// int	parse_redirections(t_token **token, t_token *cur, int flag)
// {
// 	if (!cur->next)
// 	{
// 		ft_printf("Minishell: syntax error near unexpected token`\\n\'\n");
// 		clear_list(token);
// 		return (flag + 1);
// 	}
// 	else if (cur->next && cur->next->type != WORD)
// 	{
// 		ft_printf("Minishell: syntax error near unexpected token `%s\'\n", cur->next->token);
// 		clear_list(token);
// 		return (flag + 1);
// 	}
// 	return (flag);
// }
// int    parse_separators(t_token **token, t_token *cur, int flag)
// {
// 	if (cur->next && cur->next->type >= PIPE && cur->next->type <= OR)
// 	{
// 		ft_printf("Minishell: syntax error near unexpected token `%s\'\n", 
// 			cur->next->token);
// 		clear_list(token);
// 		return (flag + 1);
// 	}
// 	return (flag);
// }
// int	parse(t_token **token)
// {
// 	t_token	*cur;
// 	int		flag;
// 	cur = *token;
// 	flag = 0;
// 	while (cur)
// 	{ 
// 		if (cur->type >= INPUT && cur->type <= APPEND_OUT)
// 			if ((flag = parse_redirections (token, cur, flag)) != 0)
// 				return (0);
// 		if (cur && cur->type >= PIPE && cur->type <= OR)
// 			if ((flag = parse_separators (token, cur, flag)) != 0)
// 				return (-1);
// 		cur = cur->next;
// 	}
// }
// int	secondquote(char *line)//rather not use
// {
// 	int		i;
// 	int		error;
// 	int		inside;
// 	char	quote;
// 	i = 0;
// 	error = 0;
// 	inside = 0;
// 	while (line[i])
// 	{
// 		if (!inside && (line[i] == '\'' || line[i] == '\"'))
// 		{
// 			quote = line[i];
// 			inside = 1;
// 			error = 1;
// 		}
// 		else if (inside == 1 && line[i] == quote)
// 		{
// 			error = 0;
// 			inside = 0;
// 		}
// 		i++;
// 	}
// 	return (error);
// }
// char	*parse_quotes(char *line)
// {
// 	int		i;
// 	char	*start;
// 	char	*newww;
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
// 					newww = ft_strjoin(line, "\n"); //not working rn
// 				else
// 					newww = ft_strjoin(line, newline);
// 				free(line);
// 				free(newline);
// 				line = newww;
// 				start = ft_strchr(line + i + 1, line[i]);
// 			}
// 				i = start - line;
// 		}
// 		i++;
// 	}
// 	return (line);
// }
// // int	parse_last_token(char **cmd_line, t_token **list, t_token **last)
// // {
// // 	char	*line;
// // 	char	*new;
// // 	char	*converted;
// // 	line = readline("> ");
// // 	if (!line)
// // 	{
// // 		clean_up_build(list, *cmd_line);
// // 		return (-1);
// // 	}
// // 	new = ft_strjoin(*cmd_line, line);
// // 	free(*cmd_line);
// // 	free(line);
// // 	*cmd_line = new;
// // 	clear_list(list);
// // 	converted = split_complex_args(*cmd_line);
// // 	tokenize(list, converted, env);
// // 	free(converted);
// // 	*last = get_last_tok(*list);
// // 	if (parse(list) == -1)
// // 	{
// // 		clean_up_build(list, *cmd_line);
// // 		return (-1);
// // 	}
// // 	return (0);
// // }

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

int parse_separators(t_token **token, t_token *cur, int flag)
{
    if (cur->next && cur->next->type >= PIPE && cur->next->type <= OR)
    {
        if (cur->type >= PIPE && cur->type <= OR) // Check for double delimiters
        {
            ft_printf("Minishell: syntax error near unexpected token `%s\'\n", cur->next->token);
            clear_list(token);
            return (flag + 1);
        }
    }
    return (flag);
}


// int	parse(t_token **token)
// {
// 	t_token	*cur;
// 	int		flag;
// 	cur = *token;
// 	flag = 0;
// 	while (cur)
// 	{ 
// 		if (cur->type >= INPUT && cur->type <= APPEND_OUT)
// 			if ((flag = parse_redirections (token, cur, flag)) != 0)
// 				return (0);
// 		if (cur && cur->type >= PIPE && cur->type <= OR)
// 			if ((flag = parse_separators (token, cur, flag)) != 0)
// 				return (-1);
// 		cur = cur->next;
// 	}
// 	return (0);
// }

int parse(t_token **token)
{
    t_token *cur;
    int flag;

    cur = *token;
    flag = 0;

    // Check for separator at the beginning of the list
    if (cur && cur->type >= PIPE && cur->type <= OR)
    {
        ft_printf("Minishell: syntax error near unexpected token `%s\'\n", cur->token);
        clear_list(token);
        return (-1);
    }

    while (cur && cur->next)
    { 
        if (cur->type >= INPUT && cur->type <= APPEND_OUT)
            if ((flag = parse_redirections(token, cur, flag)) != 0)
                return (0);
        if (cur->type >= PIPE && cur->type <= OR)
            if ((flag = parse_separators(token, cur, flag)) != 0)
                return (-1);
        cur = cur->next;
    }

    // Check for separator at the end of the list
    if (cur && cur->type >= PIPE && cur->type <= OR)
    {
        ft_printf("Minishell: syntax error near unexpected token `%s\'\n", cur->token);
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
					new = ft_strjoin(line, newline);
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
	t_env	this_has_to_exist_to_work;

	line = readline("> ");
	if (!line)
		return (clean_up_build(list, *cmd_line), -1);
	new = ft_strjoin(*cmd_line, line);
	free(*cmd_line);
	free(line);
	*cmd_line = new;
	clear_list(list);
	converted = split_complex_args(*cmd_line);
	tokenize(list, converted, this_has_to_exist_to_work);
	free(converted);
	*last = get_last_tok(*list);
	if (parse(list) == -1)
	{
		clean_up_build(list, *cmd_line);
		return (-1);
	}
	return (0);
}