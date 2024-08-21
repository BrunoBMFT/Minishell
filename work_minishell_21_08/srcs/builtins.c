/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brfernan <brfernan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 18:15:45 by bruno             #+#    #+#             */
/*   Updated: 2024/08/21 17:55:21 by brfernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
//temp_vars has to be ***?

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

int	try_builtins(t_jobs *job, char **env, char ***temp_vars, bool pipe)//wtf is bool pipe
{
	int	status;

	status = 200;
	if (ft_strcmp(job->job[0], "echo") == 0)
		status = caught_echo(job);
	if (ft_strcmp(job->job[0], "cd") == 0)//not really working with multiple jobs
		status = caught_cd(job, env);//also has to work with just ..
	else if (ft_strcmp(job->job[0], "pwd") == 0)
		status = caught_pwd(job);
	else if (ft_strcmp(job->job[0], "export") == 0)//"hello=world && export hello=mi", which stays?
		status =  caught_export(job, env, temp_vars);//take care of env-i
	else if (ft_strcmp(job->job[0], "unset") == 0)//has to unset temp_vars as well
		status = caught_unset(job, env, temp_vars);
	else if (ft_strcmp(job->job[0], "env") == 0)
		status = caught_env(job, env);
	else if (ft_strchr(job->job[0], 'exit'))
		caught_exit(job->job[0]);
	else if (ft_strchr(job->job[0], '='))
		status = declare_temp_vars(job, env, temp_vars);
	if (pipe == true)
		exit (status);
	return (status);//check if builtins fail, everything goes correct
	//exit is technically builtin
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
	while (job->job[i + 1])
	{
		ft_printf("%s ", job->job[i]);
		i++;
	}
	ft_printf("%s", job->job[i]);//find better way
	if (nl)
		ft_nl_fd(1);
	return (0);
}


char	*cd_update_aux1(char *env_var, bool when)
{
	char	cwd[PATH_MAX];
	char	*ret;
	char	*temp;
	
	getcwd(cwd, PATH_MAX);//error check
	if (when == BEFORE)
		temp = ft_strjoin("OLDPWD=", cwd);//error check
	else
		temp = ft_strjoin("PWD=", cwd);//error check
	ret = temp;
	return (ret);
/* 	int	i = 0;
	if (when == BEFORE)
	{
		while (env[i] && ft_strncmp(env[i], "OLDPWD", 6))//fix oldpwd when cd fails
			i++;
		getcwd(cwd, PATH_MAX);//error check
		temp = ft_strjoin("OLDPWD=", cwd);//error check
		env[i] = ft_strdup(temp);
	}
	else if (when == AFTER)
	{

		while (env[i] && ft_strncmp(env[i], "PWD", 3))//use envnamecmp
			i++;
		getcwd(cwd, PATH_MAX);//error check
		temp = ft_strjoin("PWD=", cwd);//error check
		env[i] = ft_strdup(temp);
	} */
}
// TODO cd can happen with just ..
int	caught_cd(t_jobs *job, char **env)//cd supposedly cant exit process, needs to be called before fork
{//check return values
	char 	*directory;
	char	*error;
	
	int i = 0;
	while (env[i] && ft_strncmp(env[i], "OLDPWD", 6))
		i++;
	env[i] = cd_update_aux1(env[i], BEFORE);
 	if (!job->job[1])
	{
		if (chdir(ft_getenv("HOME", env)) < 0)//return this
			return (ft_putendl_fd("cd home failed", 2), 1);// * need to fix perror
	}
	else
	{
		directory = job->job[1];
		if (chdir(directory) < 0)
		{
			error = ft_strjoin("minishell: cd: ", job->job[1]);//error check and free
			perror(error);
			free (error);
			return (1);// * need to fix perror
		}
	}
	 i = 0;
	while (env[i] && ft_strncmp(env[i], "PWD", 3))
		i++;
	env[i] = cd_update_aux1(env[i], AFTER);
	return (0);
}




int	caught_env(t_jobs *job, char **env)//make better
{//env SHELL var has to be different
	int	i;

	if (job->job[1])//error and exit code
	{
		ft_putendl_fd("minishell env doesnt take that many args\n", 2);//permission denied
		return (126);
	}
	i = 0;
	while (env[i])
	{
		ft_putstr_fd(env[i], 1);//needs to change i think, in the case of redirs
		ft_nl_fd(1);
		i++;
	}
	return (0);
}



int	caught_pwd(t_jobs *job)
{
	char buf[PATH_MAX];

	if (job->job[1])//error and exit code
	{
		ft_putendl_fd("pwd: too many arguments", 2);
		return (1);
	}
	ft_putendl_fd(getcwd(buf, PATH_MAX), 2);//error check?
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
