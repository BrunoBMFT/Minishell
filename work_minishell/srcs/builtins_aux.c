/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_aux.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 18:15:45 by bruno             #+#    #+#             */
/*   Updated: 2024/08/25 02:42:12 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	is_in_env(char *to_add, char **env)
{
	int	i;
	char	*temp1 = NULL;
	char	*temp2 = NULL;

	if (!to_add || !env)
		return false;
	i = 0;
	while (env[i])
	{
		temp1 = ft_strndup(to_add, len_to_equal(to_add));
		temp2 = ft_strndup(env[i], len_to_equal(env[i]));
		if (ft_strcmp(temp1, temp2) == 0)
		{
            free(temp1);
            free(temp2);
			env[i] = ft_strdup(to_add);
			return (true);
		}
        free(temp1);
        free(temp2);
		i++;
	}
	return (false);
}

int	declare_temp_vars(t_jobs *job, char **env, char ***temp_vars)
{
	int temp_vars_len = ft_split_wordcount(*temp_vars);
	int job_len = ft_split_wordcount(job->job);
	int	total_len = job_len + temp_vars_len;
	char **new_temp_vars = ft_calloc(sizeof(char *), total_len + 1);
	if (!new_temp_vars)
		return (1);//check
	int i = 0;
	while (i < temp_vars_len)
	{
		new_temp_vars[i] = ft_strdup((*temp_vars)[i]);
        if (!new_temp_vars[i])
        {
            while (i > 0)
                free(new_temp_vars[--i]);
            free(new_temp_vars);
            return (1);//error check
        }
		i++;
	}
	int j = 0;
	while (j < job_len)
	{
		if (!is_in_env(job->job[j], env))//error check
		{
			if (!is_in_env(job->job[j], new_temp_vars))//error check
			{
				new_temp_vars[i] = ft_strdup(job->job[j]);//error check
                if (!new_temp_vars[i])//make function for this
                {
                    while (i > 0)
                        free(new_temp_vars[--i]);
                    free(new_temp_vars);
                    return (1);//error check
                }
			}
		}
		j++;
		i++;
	}
	new_temp_vars[total_len] = NULL;
	*temp_vars = new_temp_vars;
	return (0);
}





void	export_new(char *var, char **env)
{
	char	*temp;
  	int i;

	i = 0;
	while (env[i])
		i++;
	if (!is_in_env(var, env))//error check, technically doesnt need to be here because if i do "varname=value, it will update env either way"
	{
		temp = ft_calloc(sizeof(char), ft_strlen(var) + 1);//error check
		ft_strcpy(temp, var);//error check
	}
	env[i] = temp;
	env[i + 1] = NULL;
}

//rename variables for better reading, error check for everything
void	export_temp(char *var, char **env, char ***temp_vars)
{
  	int i = 0;
	char *temp;
	while (env[i])
		i++;
/*	char *temp = ft_getenv(var, (*temp_vars));//gets temp env var value
	var = ft_strjoin(var, "=");//joins = sign
	temp = ft_strjoin(var, temp);//joins var name and tempenv var value
	(*env)[i] = ft_strdup(temp); */
	temp = ft_strjoin3(var, "=", ft_getenv(var, *temp_vars));//use vars[ft_strlen], error check
	if (!is_in_env(temp, env))//error check, technically doesnt need to be here because if i do "varname=value, it will update env either way"
		env[i] = ft_strdup(temp);//error check
	env[i + 1] = NULL;
}
// ! NEW LOGIC PLS
int	caught_export(t_jobs *job, char **env, char ***temp_vars)
{	
	if (!job->job[1])
		return (caught_env(job, env));
	int i = 1;
	char	*temp = NULL;
	while (job->job[i])
	{
		if (ft_strchr(job->job[i], '='))//pass &env
		{
			export_new(job->job[i], env);//error check
		}
		else
		{
			export_temp(job->job[i], env, temp_vars);//error check
		}
		i++;
	}
	return (0);
}

