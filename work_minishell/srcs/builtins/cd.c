/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 15:41:04 by brfernan          #+#    #+#             */
/*   Updated: 2024/10/24 17:16:14 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


char	**add_oldpwd(char **envp)//error check
{
	char	**new_env;
	char	*temp;
	int		i;

	i = 0;
	new_env = ft_calloc(sizeof(char *), ft_split_wordcount(envp) + 3);
	if (!new_env || !envp || !envp[0])
		return (NULL);//free new_env?
	while (envp[i])
	{
		new_env[i] = ft_strdup(envp[i]);//error check
		if (!new_env[i])
		{
			i = 0;
			while (new_env[i])
				free (new_env[i++]);//check if works
			return (NULL);
		}
		i++;
	}
	new_env[i] = ft_strdup("OLDPWD=");
    new_env[i + 1] = NULL;
	return (new_env);
}

void	cd_update_aux1(t_env *env, char *PWD, char *value)
{
	char	cwd[PATH_MAX];
    bool    found;
	int		i;

	i = 0;
    found = false;
	while (env->env[i])
    {
		if (ft_strncmp(env->env[i], PWD, ft_strlen(PWD)) == 0)
        {
            found = true;
            break ;
        }
        i++;
    }
    if (!found)
        env->env = add_oldpwd(env->env);
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

int    caught_cd(t_jobs *job, t_env *env)
{
    char     *dir;
    char    *oldpwd;
    
    oldpwd = cd_get_pwd();
     if (!job->job[1])
    {
        dir = ft_getenv("HOME", env->env);
        if (chdir(dir))
            return (ft_printf_fd(2, "cd home failed"), free (oldpwd), free (dir), 1);
    }
    else if (job->job[2])
        return (ft_printf_fd(2, "minishell: cd: too many arguments\n"), free (oldpwd), 1);
    else
    {
        dir = ft_strdup(job->job[1]);
        if (chdir(dir))
            return (ft_printf_fd(2, "minishell: cd: %s: No such file or directory\n", job->job[1]), free (oldpwd), free (dir), 1);
    }
    cd_update_aux1(env, "OLDPWD=", oldpwd);
    cd_update_aux1(env, "PWD=", NULL);
    free (dir);
    free (oldpwd);
    return (0);
}
