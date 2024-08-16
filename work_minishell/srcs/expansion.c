/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 17:53:14 by bruno             #+#    #+#             */
/*   Updated: 2024/08/16 01:03:24 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
// $? $$
char	*expand(char *str, char **env, char **temp_vars, int status)
{
	t_var_holder	h;

	h.expanded = NULL;
	h.before = NULL;
	h.temp = NULL;
	h.result = NULL;
	h.i = 0;
	h.start = 0;
	while (str[h.i])
	{
		while (str[h.i] && str[h.i] != '$')
			h.i++;
		if (h.i > h.start)
			h.result = no_expansion(str, h);
		if (str[h.i] == '$')
		{
			h.start = h.i;
			h.i++;
			h.result = expansion(str, &h, env, temp_vars, status);
		}
	}
	return (h.result);
}

char	*no_quotes(char *str, t_var_holder *h, char **env, char **temp_vars, int status)
{
	h->temp = ft_strndup(str + h->start, h->i - h->start);
	if (!h->temp)
		return h->new;
	h->before = expand(h->temp, env, temp_vars, status);
	free(h->temp);
	if (!h->before)
		return h->new;
	if (!h->new)
		h->new = ft_strdup(h->before);
	else
	{
		h->temp = ft_strjoin(h->new, h->before);
		free(h->new);
		h->new = h->temp;
	}
	free(h->before);
	if (!h->new)
		return (NULL);
	return (h->new);
}

char	*single_quotes(char *str, t_var_holder *h)
{
	h->start = ++h->i;
	while (str[h->i] && str[h->i] != '\'')
		h->i++;
	h->quoted = ft_strndup(str + h->start, h->i - h->start);
	if (!h->quoted)
		return h->new;
	h->temp = ft_strdup(h->new);
	free(h->new);
	h->new = ft_strjoin(h->temp, h->quoted);
	free(h->quoted);
	free(h->temp);
	if (!h->new)
		return (NULL);
	return (h->new);
}

char *double_quotes(char *str, t_var_holder *h, char **env, char **temp_vars, int status)
{
    h->start = ++h->i;
    while (str[h->i] && str[h->i] != '\"')
        h->i++;
    h->new = no_quotes(str, h, env, temp_vars, status);
    return (h->new);
}

char *unquote_and_direct(char *str, char **env, char **temp_vars, int status)
{
    t_var_holder h;

    h.new = NULL;
    h.before = NULL;
    h.quoted = NULL;
    h.after = NULL;
    h.temp = NULL;
    h.i = 0;
    h.start = 0;
    while (str[h.i])
    {
        h.start = h.i;
        while (str[h.i] && str[h.i] != '\'' && str[h.i] != '\"')
            h.i++;
        if (h.i > h.start) 
            h.new = no_quotes(str, &h, env, temp_vars, status);
        if (str[h.i] == '\'')
            h.new = single_quotes(str, &h);
        else if (str[h.i] == '\"') 
            h.new = double_quotes(str, &h, env, temp_vars, status);
        if (str[h.i])
            h.i++;
    }
    return (h.new);
}

char	*expand_env_vars(char *input, char **env, char **temp_vars)
{
	char	*temp;

	temp = NULL;
	temp = ft_getenv(input + 1, env);
	if (!temp && temp_vars)
		temp = ft_getenv(input, temp_vars);
	if (!temp)
		temp = ft_strdup("");
	return temp;
}

char	*no_expansion(char *str, t_var_holder h)
{
	h.before = ft_strndup(str + h.start, h.i - h.start);
	if (!h.before)
		return NULL;
	if (!h.result)
		h.temp = ft_strdup(h.before);
	else
	{
		h.temp = ft_strjoin(h.result, h.before);
		free(h.result);
	}
	free(h.before);
	return(h.temp);
}

char	*expansion(char *str, t_var_holder *h, char **env, char **temp_vars, int status)
{
	if (str[h->i] == '\0' || str[h->i] == ' ' || str[h->i] == '\t')
		h->expanded = ft_strdup("$");
	else if (str[h->i] == '$')
	{
		h->expanded = ft_itoa(getpid());//in this part, receive pid and printf pid
		h->i++;
	}
	else if (str[h->i] == '?')
	{
		h->expanded = ft_itoa(status);
//		h->expanded = ft_itoa(getpid());//in this part, receive pid and printf pid
		h->i++;
	}
	else
	{
		while (str[h->i] && (ft_isalnum(str[h->i]) || str[h->i] == '_'))
			h->i++;
		h->temp2 = ft_strndup(str + h->start, h->i - h->start);
		h->expanded = expand_env_vars(h->temp2, env, temp_vars);
		free(h->temp2);
	}
	h->temp = ft_strjoin(h->result, h->expanded);
	if (h->result)
		free(h->result);
	h->result = h->temp;
	free(h->expanded);
	h->start = h->i;
	return (h->result);
}