/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 18:15:45 by bruno             #+#    #+#             */
/*   Updated: 2024/10/07 14:27:25 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_in_env(char *to_add, char **env)
{
	int	i;
	char	*temp1 = NULL;
	char	*temp2 = NULL;

	if (!to_add || !env)
		return false;
	i = 0;
	temp1 = ft_substr(to_add, 0, len_to_equal(to_add));
	while (env[i])
	{
		temp2 = ft_substr(env[i], 0, len_to_equal(env[i]));
		if (ft_strcmp(temp1, temp2) == 0)
		{
            free(temp1);
            free(temp2);
			env[i] = ft_strdup(to_add);
			return (true);
		}
        free(temp2);
		i++;
	}
	return (free(temp1), false);
}

bool	parse_export(char *str, int n)//error messages
{
	int	i;

	if (!str || !ft_isalphaund(str[0]))
		return (false);
	i = 0;
	while (i < n)
	{
		if (!ft_isalnumund(str[i]))
			return (false);
		i++;
	}
	return (true);
}

int	export_no_execd(char **env)
{
	int		i;
	char	*temp1;
	char	*temp2;

	i = 0;
	while (env[i])
	{
		temp1 = ft_substr(env[i], 0, len_to_equal(env[i]) + 1);//error check
		temp2 = ft_substr(env[i], len_to_equal(env[i]) + 1, ft_strlen(env[i]));//error check
		ft_printf("declare -x %s\"%s\"\n", temp1, temp2);
		free (temp1);
		free (temp2);
		i++;
	}
	return (0);
}

int	caught_export(t_jobs *job, t_env *env)//fix export var =value (the space)
{
	int	status;

	if (!job->job[1])
		return (export_no_execd(env->env));//fix
	job->job++;
	status = 0;

	int oldenv_count = ft_split_wordcount(env->env);
	int var_count = ft_split_wordcount(job->job);
	char **new_env = ft_calloc(sizeof(char *), var_count + oldenv_count + 1);
	
	int i = 0;
	while (env->env[i])
	{
		new_env[i] = ft_strdup(env->env[i]);//free env[i]
		i++;
	}
	while (*job->job)
	{
		if (!parse_export(*job->job, len_to_equal(*job->job)))
		{
			ft_printf_fd(2, "minishell: export: '%s': not a valid identifier\n", *job->job);
			status = 1;//check exit code
		}
		else
		{
			if (!is_in_env(*job->job, env->env))
				new_env[i] = ft_strdup(*job->job);
		}
		i++;
		job->job++;
	}
	new_env[i] = NULL;

	//free old env
	i = 0;
	while (env->env[i])
	{
		free (env->env[i]);
		i++;
	}
	free (env->env);

	//sending to new env
	env->env = ft_calloc(sizeof(char *), ft_split_wordcount(new_env) + 1);
	i = 0;
	while (new_env[i])
	{
		env->env[i] = ft_strdup(new_env[i]);
		free (new_env[i]);
		i++;
	}
	env->env[i] = NULL;

	
/* 	//freeing new_env
	i = 0;
	while (new_env[i])
	{
		free (new_env[i]);
		i++;
	}
	free (new_env); */


	return (status);
}
