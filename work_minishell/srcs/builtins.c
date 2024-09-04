/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 18:15:45 by bruno             #+#    #+#             */
/*   Updated: 2024/09/04 02:21:41 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
//dont use bool pipe
int	try_builtins(t_jobs *job, t_env env, bool pipe)//wtf is bool pipe
{
	int	status;

	status = 200;
	if (ft_strcmp(job->job[0], "echo") == 0)//no leaks normal
		status = caught_echo(job);
/* 	if (ft_strcmp(job->job[0], "cd") == 0)
		status = caught_cd(job, env.env); */
	else if (ft_strcmp(job->job[0], "pwd") == 0)//no leaks normal
		status = caught_pwd(job);
	else if (ft_strcmp(job->job[0], "export") == 0)
		status = caught_export(job, env);
	else if (ft_strcmp(job->job[0], "unset") == 0)
		status = caught_unset(job, env);
	else if (ft_strcmp(job->job[0], "env") == 0)//no leaks normal
		status = caught_env(job, env);
	else if (ft_strchr(job->job[0], '='))
		status = declare_temp_vars(job, env);//parse
	if (pipe)
		exit (status);
	return (status);
}

int	caught_echo(t_jobs *job)//multiple strs doesnt work
{
	bool	nl;
	int		i;

	i = 1;
	nl = true;
	if (!job->job[1])
		return (ft_nl_fd(1), 0);
	if (ft_strcmp(job->job[1], "-n") == 0)
	{
		nl = false;
		i++;
	}
	//echo ola   ""    bruno
	while (job->job[i])
	{
		if (job->job[i][0])
			printf("%s", job->job[i]);
		if (job->job[i + 1])
			printf(" ");
		i++;
	}
	if (nl)
		printf("\n");
	return (0);
}

void	cd_update_aux1(char **env, char *PWD, char *value)
{
	char	cwd[PATH_MAX];
	char	*temp;
	int i = 0;

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
//dont have to handle ..
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

int	unset_aux(char **to_remove, char **env)
{
	int	i;

	while (*to_remove)
	{
		i = 0;
		if (!env)
			return (1);
		while (env && env[i] && ft_strncmp(env[i], *to_remove, ft_strlen(*to_remove)))
			i++;
		if (!env[i])
			return (1);
		free (env[i]);//not freeing from temp_vars
		while (env[i])
		{
			env[i] = env[i + 1];
			i++;
		}
		to_remove++;
	}
	return (0);
}

int	caught_unset(t_jobs *job, t_env env)
{
	if (!job->job[1])
		return (0);
	if (unset_aux(job->job + 1, env.env))
		unset_aux(job->job + 1, *env.temp_vars);
	return (0);
}

int	caught_exit(t_jobs *job, t_env env)
{
	char *error;

	if (job->job[1] && !job->job[2])
	{
		// TODO fix
//		if (ft_isdigit(ft_atoi(job->job[1])))
//		{
//			printf("here\n");
//			exit (ft_atoi(job->job[1]));
//		}
		//error check
		error = ft_strjoin3("minishell: exit: ", job->job[1], ": numeric argument required");
		ft_putendl_fd(error, 2);
		free (error);
		exit (0);//tester says exit code has to be 0
	}
	if (job->job[1] && job->job[2])
		return (ft_putendl_fd("minishell: exit: too many arguments", 2), 0);//tester says exit code has to be 0??
	printf("exit\n");
	free_array(*env.temp_vars);
	free_array(env.env);
	rl_clear_history();
	clear_jobs(&job);
	exit(0);
}

int	caught_env(t_jobs *job, t_env env)
{
	int		i;
	char	*error;

	if (job->job[1])//error and exit code
	{
		error = ft_strjoin3("env: '", job->job[1], "': No such file or directory");
		ft_putendl_fd(error, 2);//hardcoded
		free (error);
		return (127);
	}
	i = 0;
	while (env.env[i])
	{
		ft_putendl_fd(env.env[i], 1);
		i++;
	}
	return (0);
}

int	caught_pwd(t_jobs *job)
{
	char cwd[PATH_MAX];

	if (!getcwd(cwd, PATH_MAX))
		return (printf("getcwd() error"), 127);//exit code?
	ft_putendl_fd(cwd, 1);//error check?
	return (0);
}