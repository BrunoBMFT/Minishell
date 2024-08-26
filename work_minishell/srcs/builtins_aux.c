/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_aux.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brfernan <brfernan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 18:15:45 by bruno             #+#    #+#             */
/*   Updated: 2024/08/26 20:27:42 by brfernan         ###   ########.fr       */
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

bool	declare_temp_vars_parse(t_jobs *job)
{
	int	i;//increment with pointers?

	int k = 0;
	while (job->job[k])
	{
		i = 0;
		if (!ft_isalpha(job->job[k][i]))//make error message better
			return (ft_putendl_fd(ft_strjoin(job->job[k], ": command not found"), 2), false);
		while (i < len_to_equal(job->job[k]))
		{
			if (!ft_isalnum(job->job[k][i]))
				return (ft_putendl_fd(ft_strjoin(job->job[k], ": command not found"), 2), false);	
			i++;
		}
		k++;
	}
	return (true);
}

int	declare_temp_vars(t_jobs *job, char **env, char ***temp_vars)//parse
{
	int temp_vars_len;
	int job_len;
	char **new_temp_vars;
	
	if (!declare_temp_vars_parse(job))
		return (127);
	temp_vars_len = ft_split_wordcount(*temp_vars);
	job_len = ft_split_wordcount(job->job);
	new_temp_vars = ft_calloc(sizeof(char *), temp_vars_len + job_len + 1);
	if (!new_temp_vars)
		return (1);
	int i = 0;
	while (i < temp_vars_len)
	{
		new_temp_vars[i] = ft_strdup((*temp_vars)[i]);
        if (!new_temp_vars[i])//test
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
                if (!new_temp_vars[i])//make function for this, test
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
	new_temp_vars[job_len + temp_vars_len] = NULL;
	*temp_vars = new_temp_vars;
	return (0);
}





void	export_new(char *var, char **env)
{
  	int i;

	i = 0;
	while (env[i])
		i++;
	if (!is_in_env(var, env))//error check, technically doesnt need to be here because if i do "varname=value, it will update env either way"
	{
		env[i] = ft_strdup(var);//error check
	}
	env[i + 1] = NULL;
}
// TODO test temp
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

bool	export_is_alnum(char *str, int n)
{
	int	i;

	if (!str || !ft_isalpha(str[0]))
		return (false);
	i = 0;
	while (i < n)
	{
		if (!ft_isalnum(str[i]))
			return (false);
		i++;
	}
	return (true);
}

int	caught_export(t_jobs *job, char **env, char ***temp_vars)//fix export var =value (the space)
{
	int	status;

	if (!job->job[1])
		return (caught_env(job, env));
	job->job++;
	status = 0;
	while (*job->job)
	{
		if (!export_is_alnum(*job->job, len_to_equal(*job->job)))//seems correct
		{
			ft_putendl_fd(ft_strjoin3("minishell: export: '",
							*job->job, "': not a valid identifier"), 2);//error check
			status = 1;
		}
		else if (ft_strchr(*job->job, '='))//pass &env
		{
			export_new(*job->job, env);//error check
		}
		else
		{
			export_temp(*job->job, env, temp_vars);//error check
		}
		job->job++;
	}
	return (status);
}

