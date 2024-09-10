/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 15:41:04 by brfernan          #+#    #+#             */
/*   Updated: 2024/09/08 17:39:48 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	cd_update_aux1(t_env env, char *PWD, char *value)
{
	char	cwd[PATH_MAX];
	int		i = 0;

	while (env.env[i] && ft_strncmp(env.env[i], PWD, ft_strlen(PWD)))//find better way
		i++;
	if (!value)
		value = getcwd(cwd, PATH_MAX);//error check
	free (env.env[i]);
	env.env[i] = ft_strjoin(PWD, value);
	if (!env.env[i])
		return ((void)NULL);
}

char	*cd_get_pwd(void)
{
	char	cwd[PATH_MAX];
	char	*ret;

	if (!getcwd(cwd, PATH_MAX))
		return (ft_printf_fd(2, "getcwd() error\n"), NULL);
	ret = ft_strdup(cwd);
//	ret = NULL;
	if (!ret)
		return (NULL);
	return (ret);
}

int	caught_cd(t_jobs *job, t_env env)
{
	char 	*directory;
	char	*oldpwd;
	
	oldpwd = cd_get_pwd();
 	if (!job->job[1])
	{
		if (chdir(ft_getenv("HOME", env.env)))
			return (ft_printf_fd(2, "cd home failed"), free (oldpwd), 1);
	}
	else
	{
		directory = job->job[1];
		if (chdir(directory))
			return (ft_printf_fd(2, "minishell: cd: %s: No such file or directory\n", job->job[1])
						, free (oldpwd), 1);
	}
	cd_update_aux1(env, "OLDPWD=", oldpwd);
	cd_update_aux1(env, "PWD=", NULL);
	free (oldpwd);
	return (0);
}
//	echo new:$PWD "     " old:$OLDPWD