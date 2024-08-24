/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 18:15:45 by bruno             #+#    #+#             */
/*   Updated: 2024/08/24 03:44:52 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	try_builtins(t_jobs *job, char **env, char ***temp_vars, bool pipe)//wtf is bool pipe
{
	int	status;

	status = 200;
	if (ft_strcmp(job->job[0], "echo") == 0)
		status = caught_echo(job);
	if (ft_strcmp(job->job[0], "cd") == 0)
		status = caught_cd(job, env);
	else if (ft_strcmp(job->job[0], "pwd") == 0)
		status = caught_pwd(job);
	else if (ft_strcmp(job->job[0], "export") == 0)
		status =  caught_export(job, env, temp_vars);
	else if (ft_strcmp(job->job[0], "unset") == 0)
		status = caught_unset(job, env, temp_vars);
	else if (ft_strcmp(job->job[0], "env") == 0)
		status = caught_env(job, env);
	else if (ft_strcmp(job->job[0], "exit") == 0)// TODO fix
			caught_exit(job->job[0]);
	else if (ft_strchr(job->job[0], '='))
		status = declare_temp_vars(job, env, temp_vars);
	if (pipe == true)
		exit (status);
	return (status);
}

int	caught_echo(t_jobs *job)//multiple strs doesnt work
{
	//redirs not working
	bool	nl;
	int		i;

	i = 1;
	if (!job->job[1])
		return (ft_nl_fd(1), 0);
	nl = true;
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


void	cd_update_aux1(char **env, char *PWD)
{
	char	cwd[PATH_MAX];
	char	*ret;
	char	*temp;
	int i = 0;

	while (env[i] && ft_strncmp(env[i], PWD, ft_strlen(PWD)))//find better way
		i++;
	getcwd(cwd, PATH_MAX);//error check
	env[i] = ft_strjoin(PWD, cwd);//error check
}

int	caught_cd(t_jobs *job, char **env)
{
	char 	*directory;
	char	*error;

	cd_update_aux1(env, "OLDPWD=");// TODO cant run if this fails
 	if (!job->job[1])
	{
		if (chdir(ft_getenv("HOME", env)) < 0)//error check
			return (ft_putendl_fd("cd home failed", 2), 1);
	}
	else
	{
		directory = job->job[1];
		if (chdir(directory) < 0)
		{
			error = ft_strjoin("minishell: cd: ", job->job[1]);//error check
			perror(error);
			free (error);
			return (1);
		}
	}
	cd_update_aux1(env, "PWD=");
	return (0);
}

int	unset_aux(char **to_remove, char **env)
{
// TODO has to unset temp_vars
	int		i;
	while (*to_remove)
	{
		i = 0;
		while (env[i] && ft_strncmp(env[i], *to_remove, ft_strlen(*to_remove)))
			i++;
		if (!env[i])
			return (1);
		while (env[i])
		{
			env[i] = env[i + 1];
			i++;
		}
		to_remove++;
	}
	return (0);
}

int	caught_unset(t_jobs *job, char **env, char ***temp_vars)//segfault if var doesnt exist
{
	int		i;
	char	**to_remove;

	to_remove = ft_split(job->job[1], ' ');//error check
	if (unset_aux(to_remove, env))
		unset_aux(to_remove, *temp_vars);
	return (0);
}


void	caught_exit(char *line)//check pipes
{// TODO cant exit if it's piped (exit | exit)
	printf("exit\n");
	if (ft_strcmp(line, "exit") == 0)
	{
		free(line);//no line to clear
		rl_clear_history();
		exit(0);
	}
}





int	caught_env(t_jobs *job, char **env)// TODO Almost Done
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
	while (env[i])
	{
		ft_putendl_fd(env[i], 1);
		i++;
	}
	return (0);
}

int	caught_pwd(t_jobs *job)// TODO almost done
{
	char cwd[PATH_MAX];

/* 	if (job->job[1])
	{
		ft_putendl_fd("pwd: too many arguments", 2);
		return (1);
	} */
	if (!getcwd(cwd, PATH_MAX))
		return (printf("getcwd() error"), 127);//exit code?
	ft_putendl_fd(cwd, 2);//error check?
	return (0);
}