/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brfernan <brfernan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 15:41:04 by brfernan          #+#    #+#             */
/*   Updated: 2024/09/05 20:33:41 by brfernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	cd_update_aux1(char **env, char *PWD, char *value)
{
	char	cwd[PATH_MAX];
	int		i = 0;

	while (env[i] && ft_strncmp(env[i], PWD, ft_strlen(PWD)))//find better way
		i++;
	if (!value)
		value = getcwd(cwd, PATH_MAX);//error check
	env[i] = ft_strjoin(PWD, value);//error check
	if (!env[i])
		return (free (env[i]), (void)NULL);
}

char	*cd_get_pwd(char **env)
{
	char	cwd[PATH_MAX];
	char	*ret;

	getcwd(cwd, PATH_MAX);//error check
	ret = ft_strdup(cwd);//error check
	return (ret);
}

int	caught_cd(t_jobs *job, char **env)
{
	char 	*directory;
	char	*error;
	char	*oldpwd;
	
	oldpwd = cd_get_pwd(env);
 	if (!job->job[1])
	{
		if (chdir(ft_getenv("HOME", env)) < 0)//error check
			return (ft_putendl_fd("cd home failed", 2), 0);//tester says exit code has to be 0
	}
	else
	{
		directory = job->job[1];
		if (chdir(directory) < 0)
		{
			error = ft_strjoin("minishell: cd: ", job->job[1]);//error check
			return (perror(error), free (error), free (oldpwd), 0);//tester says exit code has to be 0
		}
	}
	cd_update_aux1(env, "OLDPWD=", oldpwd);
	cd_update_aux1(env, "PWD=", NULL);
	free (oldpwd);
	return (0);
}