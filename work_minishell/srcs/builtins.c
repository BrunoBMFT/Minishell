/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 18:15:45 by bruno             #+#    #+#             */
/*   Updated: 2024/07/19 20:15:44 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
//check env vars problems (cd includesdadasd OR unset PATHASDHASDHASD)


int	try_builtins(t_jobs *job, char **env)
{
	if (ft_strncmp(job->cmd, "cd", 2) == 0)// fix to use execd
		return (caught_cd(job, env));
	if (ft_strncmp(job->cmd, "echo", 4) == 0)// fix to use execd
		return (caught_echo(job));
	else if (ft_strncmp(job->cmd, "env", 3) == 0)
		return (caught_env(job, env));
	else if (ft_strncmp(job->cmd, "pwd", 3) == 0)
		return (caught_pwd(job, env));
	else if (ft_strncmp(job->cmd, "unset", 5) == 0)
		return (caught_unset(job, env));
	else if (ft_strncmp(job->cmd, "clear", 5) == 0)// * REMOVE
		return (clear_proc(env));// * REMOVE
	return (200);
}

int	caught_unset(t_jobs *job, char **env)
{
	int		i;
	char	**to_remove;

	to_remove = ft_split(job->execd, ' ');//error check
	while (*to_remove)
	{
		i = 0;
		while (env[i] && ft_strncmp(env[i], *to_remove, ft_strlen(*to_remove)))
			i++;
		if (!env[i])
			return (0);
		while (env[i])
		{
			env[i] = env[i + 1];
			i++;
		}
		to_remove++;
	}
	
	return (0);
}

// cd 
void	cd_update_pwd(char **env, bool when)
{
	char	cwd[100];
	char	*temp;

	int	i = 0;
	if (when == BEFORE)
	{
		while (env[i] && ft_strncmp(env[i], "OLDPWD", 6))//fix oldpwd when cd fails
			i++;
		temp = getcwd(cwd, sizeof(cwd));//error check
		env[i] = ft_strjoin("OLDPWD=", temp);//error check
	}
	else if (when == AFTER)
	{
		while (env[i] && ft_strncmp(env[i], "PWD", 3))
			i++;
		temp = getcwd(cwd, sizeof(cwd));//error check
		env[i] = ft_strjoin("PWD=", temp);//error check
	}
}

int	caught_cd(t_jobs *job, char **env)//check return values
{
	char 	*directory;
	
	cd_update_pwd(env, BEFORE);
	directory = job->execd;
 	if (!job->execd)
	{
		if (chdir(ft_getenv("HOME", env)) < 0)
			return (ft_putendl_fd("cd home failed", 2), 1);// * need to fix perror
	}
	if (chdir(directory) < 0)
		return (ft_putendl_fd("cd failed", 2), 1);// * need to fix perror
	cd_update_pwd(env, AFTER);
	return (0);
}

//env
int	caught_env(t_jobs *job, char **env)
{
	int	i;

	if (ft_strlen(job->cmd) > 4)//error and exit code
	{
		ft_putendl_fd("minishell env doesnt take that many args\n", 2);//permission denied
		return (126);
	}
	i = 0;
	while (env[i])
	{
		ft_putstr_fd(env[i], 1);
		ft_nl_fd(1);
		i++;
	}
	return (0);
}
//pwd
// ! AFTER UNSETTING PWD, THE PWD COMMAND HAS TO STILL WORK
int	caught_pwd(t_jobs *job, char **env)//
{
	int		i;
	char	*pwd;

	if (job->execd)//error and exit code
	{
		ft_putendl_fd("pwd: too many arguments", 2);
		return (1);
	}
	i = 0;
	while (env[i] && ft_strncmp(env[i], "PWD", 3))
		i++;
	if (!env[i])
		return (1);
	pwd = ft_strrem(env[i], "PWD=");
	ft_putendl_fd(pwd, 2);//error check?
	free (pwd);
	return (0);
}

//echo
int	caught_echo(t_jobs *job)//! FIX
{
	bool	nl;
	
	nl = true;
	if (ft_strncmp(job->cmd, "echo -n ", 8) == 0)
		nl = false;
	else if (ft_strncmp(job->cmd, "echo -n", 7) == 0)
		ft_printf("%s", job->cmd + 5);//error check?
	else
		ft_printf("%s", job->execd);
	if (nl == true)
		ft_nl_fd(1);
	return (0);
}
