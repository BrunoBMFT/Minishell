/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 03:18:24 by bruno             #+#    #+#             */
/*   Updated: 2024/07/27 20:59:27 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*expand_env_vars(char *input, char **env, char **temp_vars)//take care of $$
{
	int 	i;
	bool	flag = false;
	char	**vars = ft_split(input, '$');
	if (!vars)
		return (NULL);
	char	*output = NULL;
	char	*temp = NULL;

	if (input[0] != '$')
		flag = true;
	i = 0;
	while (vars[i])
	{
		if (flag)
			flag = false;// dont hardcode like this
		else
		{
			temp = ft_getenv(vars[i], env);//error check
			if (!temp)
				temp = ft_getenv(vars[i], temp_vars);//error check
			vars[i] = temp;
		}
		if (vars[i])
		{
			if (!output)
				output = ft_strdup(vars[i]);//error check
			else
				output = ft_strjoin(output, vars[i]);//error check
		}
		i++;
	}
	free_array(vars);
	if (input[ft_strlen(input) - 1] == '$')//! dont hardcode like this
		ft_strcat(output, "$");
	output[ft_strlen(output)] = 0;
	return (output);
}

char	**add_to_env(char **strs, char **temp_vars)
{
	char	**new_env;
	new_env = ft_calloc(sizeof(char *), ft_split_wordcount(temp_vars) + ft_split_wordcount(strs) + 1);//errorcheck
	int i = 0;
	if (temp_vars)
	{
		while (temp_vars[i])
		{
			new_env[i] = ft_strdup(temp_vars[i]);
			i++;
		}
	}
	while (*strs)
	{
		int j = 0;
		bool found = false;
		while (new_env[j])
        {
            if (ft_strncmp(new_env[j], *strs, len_to_equal(new_env[j]) + 1) == 0) 
			{
                new_env[j] = ft_strdup(*strs);
                found = true;
                break;
            }
			j++;
        }
        if (found == false)
            new_env[i] = ft_strdup(*strs);
		i++;
		strs++;
	}
	new_env[i] = NULL;
	return (new_env);
}

/* char	**variable_declaration(char **str, char **vars)//fix "export hello=world && hello=hi", env hello has to become hi
{
	char **temp_vars;
	int		var_count = ft_split_wordcount(vars);
	int		str_count = ft_split_wordcount(str);
	temp_vars = ft_calloc(sizeof(char *), var_count + str_count + 1);
	if (!temp_vars)
		panic("malloc\n");
	int i = 0;
	while (i < var_count)
	{
        bool found = false;
		int j = 0;
		while (j < str_count)
        {
            if (ft_strncmp(vars[i], str[j], len_to_equal(vars[i])) == 0) 
			{
                temp_vars[i] = ft_strdup(str[j]);
                found = true;
                break;
            }
			j++;
        }
        if (found == false)
            temp_vars[i] = ft_strdup(vars[i]);
		i++;
    }
	int j = 0;
	while (j < str_count)
	{
        bool found = false;
		int k = 0;
		while (k < var_count)
        {
            if (ft_strncmp(str[j], vars[k], len_to_equal(vars[k])) == 0) 
			{
                found = true;
                break;
            }
			k++;
        }
        if (found == false)
            temp_vars[i++] = ft_strdup(str[j]);
		j++;
    }
	temp_vars[var_count + str_count] = NULL;
	return (temp_vars);
} */

