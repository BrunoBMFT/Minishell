/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 15:41:04 by brfernan          #+#    #+#             */
/*   Updated: 2024/10/25 02:30:59 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	cd_update_aux1(t_env *env, char *PWD, char *value)
{
	char	cwd[PATH_MAX];
	int		i;

	i = 0;
	while (env->env[i]
		&& ft_strncmp(env->env[i], PWD, ft_strlen(PWD)))
		i++;
	if (!value)
		value = getcwd(cwd, PATH_MAX);
	free (env->env[i]);
	env->env[i] = ft_strjoin(PWD, value);
	if (!env->env[i])
		return ((void) NULL);
}

char	*cd_get_pwd(void)
{
	char	cwd[PATH_MAX];
	char	*ret;

	if (!getcwd(cwd, PATH_MAX))
		return (ft_printf_fd(2, "getcwd() error\n"), NULL);
	ret = ft_strdup(cwd);
	if (!ret)
		return (NULL);
	return (ret);
}

void	cd_error(char *str, char **oldpwd, char **dir, t_env *env)
{
	if (str)
	{
		ft_printf_fd(2, "minishell: cd: %s\n", str);
		free (str);//
	}
	if (dir)
		free (*dir);
	if (oldpwd)
		free (*oldpwd);
	env->status = 1;
}

void	caught_cd(t_jobs *job, t_env *env)
{
	char	*dir;
	char	*oldpwd;

	if (job->job[1] && job->job[2])
		return (cd_error(ft_strdup("too many arguments"), NULL, NULL, env));
	oldpwd = cd_get_pwd();
	if (!job->job[1])
	{
		dir = ft_getenv("HOME", env->env);
		if (chdir(dir))
			return (cd_error(ft_strdup("~: No such file or directory")
					, &oldpwd, &dir, env));
	}
	else
	{
		dir = ft_strdup(job->job[1]);
		if (chdir(dir))
			return (cd_error(ft_strjoin(job->job[1]
						, ": No such file or directory"), &oldpwd, &dir, env));
	}
	cd_update_aux1(env, "OLDPWD=", oldpwd);
	cd_update_aux1(env, "PWD=", NULL);
	free (dir);
	free (oldpwd);
	return ;
}
