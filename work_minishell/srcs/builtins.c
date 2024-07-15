/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 18:15:45 by bruno             #+#    #+#             */
/*   Updated: 2024/07/11 22:51:34 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	execute_builtins(t_jobs *job, char **env)
{
	if (ft_strncmp(job->cmd, "cd", 2) == 0)// fix to use execd
		return (caught_cd(job, env), 1);
	if (ft_strnstr(job->cmd, "echo", 4))// fix to use execd
		return (caught_echo(job), true);
	else if (ft_strnstr(job->cmd, "env", 3))
		return (caught_env(job, env), true);
	else if (ft_strnstr(job->cmd, "pwd", 3))
		return (caught_pwd(job, env), true);
	return (false);
	//not good for the cases where cd fails and returns exit code, fix 
}

//cd
void	update_pwd(char **env, bool when)
{
	char	cwd[100];
	char	*temp;

	int	i = 0;
	if (when == BEFORE)
	{
		while (env[i] && ft_strncmp(env[i], "OLDPWD", 6) != 0)//fix oldpwd when cd fails
			i++;
		temp = getcwd(cwd, sizeof(cwd));//error check
		env[i] = ft_strjoin("OLDPWD=", temp);//error check
	}
	else
	{
		while (env[i] && ft_strncmp(env[i], "PWD", 3) != 0)
			i++;
		temp = getcwd(cwd, sizeof(cwd));//error check
		env[i] = ft_strjoin("PWD=", temp);//error check
	}
}

void	caught_cd(t_jobs *job, char **env)
{
	char 	*directory;
	
	update_pwd(env, BEFORE);
	directory = job->execd;
 	if (!job->execd)
	{
		if (chdir(ft_getenv("HOME", env)) < 0)//error and exit code, dont use getenv
			return (printf("cd home failed\n"), (void)NULL);// * need to fix perror
		else
			return ;
	}
	if (chdir(directory) < 0)
		return (printf("cd failed\n"), (void)NULL);// * need to fix perror
	update_pwd(env, AFTER);
	return ;
}

//env
void	caught_env(t_jobs *job, char **env)
{
	int	i;

	if (ft_strlen(job->cmd) > 4)//error and exit code
	{
		printf("minishell env doesnt take that many args\n");
		exit (1);
	}
	i = 0;
	while (env[i])
	{
		ft_printf("%s\n", env[i]);
		i++;
	}
	exit (0);
}
//pwd
void	caught_pwd(t_jobs *job, char **env)//test with unset pwd
{
	int		i;
	char	*pwd;

	if (job->execd)//error and exit code
	{
		printf("pwd: too many arguments\n");
		exit (1);
	}
	i = 0;
	while (env[i] && ft_strnstr(env[i], "PWD", 3) == 0)
		i++;
	if (!env[i])
		exit (1);
	pwd = ft_strrem(env[i], "PWD=");
	ft_printf("%s\n", pwd);//error check?
	free (pwd);
	exit (0);
}

//echo
void	caught_echo(t_jobs *job)//fix echo "   hello"
{
	bool	nl;
	
	nl = true;
	if (ft_strnstr(job->cmd, "-n", 7))
		nl = false;
	ft_printf("%s", job->execd);//error check?
	if (nl == true)
		ft_nl_fd(1);
	exit(0);
}
