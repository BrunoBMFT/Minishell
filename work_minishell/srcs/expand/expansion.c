/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 17:53:14 by bruno             #+#    #+#             */
/*   Updated: 2024/10/13 21:35:53 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*expand(char *str, t_env *env)
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
			h.result = expansion(str, &h, env);
		}
	}
	return (h.result);
}

char	*no_quotes(char *str, t_var_holder *h, t_env *env)
{
	h->temp = ft_strndup(str + h->start, h->i - h->start);
	if (!h->temp)
		return (h->new);
	h->before = expand(h->temp, env);
	free(h->temp);
	if (!h->before)
		return (h->new);
	if (!h->new)
		h->new = ft_strdup(h->before);//this is leaked in export????
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
	if (!h->new)
		h->new = ft_strdup("");
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

char *double_quotes(char *str, t_var_holder *h, t_env *env)
{
    h->start = ++h->i;
    while (str[h->i] && str[h->i] != '\"')
        h->i++;
    h->new = no_quotes(str, h, env);
    return (h->new);
}

bool	is_empty_arg(char *str, char end)
{
	int	i;
	int	found;

	i = 0;
	found = 1;
	while (str[i] && str[i] != end)
	{
		if (ft_isascii(str[i]) && str[i] != '\0')//check for spaces as well?
			return (true);
		i++;
	}
	return (false);
}

char *unquote_and_direct(char *str, t_env *env)
{
	t_var_holder h;

	h.new = NULL;
	h.before = NULL;
	h.quoted = NULL;
	h.after = NULL;
	h.temp = NULL;
	h.i = 0;
	h.start = 0;
	//check if the if statements are correct, basically it gets triggered at the wrong timing
	
	if (is_empty_arg(str, '\'') || is_empty_arg(str, '\"'))
	{
		h.new = ft_strdup("");
	}
	while (str[h.i])
	{
		h.start = h.i;
		while (str[h.i] && str[h.i] != '\'' && str[h.i] != '\"')
			h.i++;
		if (h.i > h.start)
			h.new = no_quotes(str, &h, env);
		if (str[h.i] == '\'')
			h.new = single_quotes(str, &h);
		else if (str[h.i] == '\"') 
			h.new = double_quotes(str, &h, env);
		if (str[h.i])
			h.i++;
	}
	return (h.new);
}

char	*expand_env_vars(char *input, t_env *env)
{
	char	*temp;

	temp = ft_getenv(input + 1, env->env);
//	if (!temp)
//		temp = ft_strdup("");keeping it at NULL???
	return (temp);
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

char	*expansion(char *str, t_var_holder *h, t_env *env)
{
	if (str[h->i] == '$')//elif
	{
		h->expanded = ft_itoa(ft_getpid());//check if ft_getpid is working
		h->i++;
	}
	else if (str[h->i] == '?')
	{
		h->expanded = ft_itoa(env->status);
		h->i++;
	}
	else
	{
		while (str[h->i] && (ft_isalnum(str[h->i]) || str[h->i] == '_'))
			h->i++;
		h->temp2 = ft_strndup(str + h->start, h->i - h->start);
		h->expanded = expand_env_vars(h->temp2, env);
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
