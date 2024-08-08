/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 03:18:24 by bruno             #+#    #+#             */
/*   Updated: 2024/08/08 22:40:28 by bruno            ###   ########.fr       */
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

int temp_env_to_env(char **new_env, char **temp_vars)//check duplicates in normal env
{
	bool	found;
	int		i = 0, j = 0;
	
	while (*temp_vars)
	{
		found = false;
		int j = 0;
		while (!found && new_env[j])
		{
			if (ft_envnamecmp(new_env[j], *temp_vars) == 0)
			{
				new_env[j] = ft_strdup(*temp_vars);
				if (!new_env[i])
					panic ("strdup1 temp_vars\n");
				found = true;
				break;
			}
			j++;
		}
        if (!found) {
            new_env[i] = ft_strdup(*temp_vars);
            if (!new_env[i])
                panic ("strdup2 env\n");
		}
		temp_vars++;
		i++;
	}
	return (i);//find a way for the 2nd one to work without returning this pointer
}

int	temp_vars_to_env(char **new_env, char **vars, char **env, int i)
{
/* 	while (i < ft_split_wordcount(env))
	{
		printf("here\n");
		if (ft_envnamecmp(new_env[i], *env) == 0)//find better logic
		{
			new_env[i] = ft_strdup(*env);
			if (!new_env[i])
				panic ("strdup1vars \n");
		}
		else if (ft_envnamecmp(new_env[i], *vars) == 0)//find better logic
		{
			new_env[i] = ft_strdup(*vars);
			if (!new_env[i])
				panic ("strdup1vars \n");
		}
		i++;
	}
	while (*vars && new_env[i])
	{
		new_env[i] = ft_strdup(*vars);//check order
		if (!new_env[i])
			panic ("strdup2 vars\n");
		i++;
	} */
	while (*vars)
	{
		bool found = false;
		int j = 0;
		while (new_env[j])
		{
			if (ft_envnamecmp(new_env[j], *vars) == 0)//find better logic
			{
                new_env[j] = ft_strdup(*vars);
            	if (!new_env[j])
               		panic ("strdup1vars \n");
				found = true;
				break;
			}
			j++;
		}
		if (!found) {
			new_env[i] = ft_strdup(*vars);
			if (!new_env[i])
				panic ("strdup2 vars\n");
		}
		vars++;
		i++;
	}
	new_env[i] = NULL;
	return (i);
}

char	**vars_declaration(char *input, char **temp_vars, char **env)//minishell env has to import all variables declared in shell
{//parse
	char	**strs = ft_split(input, ' ');
	if (!strs)
		panic ("strs split\n");
	char	**new_env = ft_calloc(sizeof(char *), ft_split_wordcount(temp_vars) + ft_split_wordcount(strs));//error check

	int i = 0;
	if (temp_vars)
		i = temp_env_to_env(new_env, temp_vars);//error check, find better way
	if (strs)
		temp_vars_to_env(new_env, strs, env, i);//error check
	return (new_env);
}
//import minishell tempvars
//to check if a new variable was declared, it should do a strncmp till = sign to see if there is a variable with that name
