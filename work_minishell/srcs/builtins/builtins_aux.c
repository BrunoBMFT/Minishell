/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_aux.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 18:15:45 by bruno             #+#    #+#             */
/*   Updated: 2024/09/06 20:13:25 by bruno            ###   ########.fr       */
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

void	export_new(char *var, char **env)
{
	char	**new_env;
	int		i;

	if (is_in_env(var, env))
		return ;
	i = 0;
	new_env = ft_calloc(sizeof(char *), ft_split_wordcount(env) + 1);
	if (!new_env)
		return ((void)NULL);
	new_env = env;
	while (env[i])
		i++;
	new_env[i] = ft_strdup(var);//error check
	new_env[i + 1] = NULL;
}

int	caught_export(t_jobs *job, t_env env)//fix export var =value (the space)
{
	int	status;

	if (!job->job[1])
		return (caught_env(job, env));//fix
	job->job++;
	status = 0;
	while (*job->job)
	{
		if (!parse_export(*job->job, len_to_equal(*job->job)))//seems correct
		{
			ft_printf_fd(2, "minishell: export: '%s': not a valid identifier\n", *job->job);
			status = 1;//tester says exit code has to be 0
		}
		else if (ft_strchr(*job->job, '='))//pass &env
		{
			export_new(*job->job, env.env);//error check
		}
		job->job++;
	}
	return (status);
}

