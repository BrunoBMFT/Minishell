/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 03:18:24 by bruno             #+#    #+#             */
/*   Updated: 2024/08/01 22:17:51 by bruno            ###   ########.fr       */
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
	if (output)
		output[ft_strlen(output)] = 0;
	else
		output = "";
	return (output);
}

char	**add_to_env(char **strs, char **env)//minishell env has to import all variables declared in shell
{
	char	**new_env;

	new_env = ft_calloc(sizeof(char *), ft_split_wordcount(env) + ft_split_wordcount(strs) + 1);//errorcheck
	if (!new_env)
		panic ("env calloc\n");
	int i = 0;
	if (env) {
		while (env[i]) {
			new_env[i] = ft_strdup(env[i]);
			if (!new_env[i])
				panic ("env dup\n");
			i++;
		}
	}
	while (*strs) {
		new_env[i] = ft_strdup(*strs);
		if (!new_env[i])
			panic ("dup strs\n");
		i++;
		strs++;
	}
	new_env[i] = NULL;
	return (new_env);
}
/* char	**old_add_to_env(char **strs, char **temp_vars, char **env)
{
	char	**new_env;

	new_env = ft_calloc(sizeof(char *), ft_split_wordcount(temp_vars) + ft_split_wordcount(env) + ft_split_wordcount(strs) + 1);//errorcheck
	if (!new_env)
		panic ("1\n");
	int i = 0, j = 0;
	if (env) {
        while (env[i]) {
            new_env[i] = ft_strdup(env[i]);
            if (!new_env[i])
                return NULL;
            i++;
        }
    }
	if (temp_vars) {
		while (temp_vars[j]) {
			new_env[i] = ft_strdup(temp_vars[j]);
            if (!new_env[i])
                panic ("3\n");
			i++;
			j++;
		}
	}
	
	while (*strs) {
		j = 0;
		bool found = false;
		while (new_env[j]) {
            if (ft_strncmp(new_env[j], *strs, len_to_equal(new_env[j])) == 0) {
				free(new_env[j]);
                new_env[j] = ft_strdup(*strs);
        	    if (!new_env[j])
             	   panic ("4\n");
                found = true;
                break;
            }
			j++;
        }
        if (found == false){
            new_env[i] = ft_strdup(*strs);
            if (!new_env[i])
                panic ("5\n");
		}
			i++;
		strs++;
	}
	new_env[i] = NULL;
	i = 0;
	while (env && env[i]) {
		env[i] = ft_strdup(new_env[i]);
		if (!env[i])
			panic ("6\n"); // Handle allocation error
		i++;
	}
	i = 0;
	while (ft_strncmp(env[i], new_env[i], len_to_equal(new_env[i])) == 0)//not working, trying this to not dup env every time
		i++;
	while (new_env[i])
	{
		new_env[j] = ft_strdup(new_env[i]);
		if (!new_env[j])
			panic ("7\n"); // Handle allocation error
		i++;
		j++;
	}
	new_env[j] = NULL;
	return (new_env);
} */
