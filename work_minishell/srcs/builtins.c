/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 18:15:45 by bruno             #+#    #+#             */
/*   Updated: 2024/08/19 02:12:20 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
//NOT WORKING HAHAHAHAHAHAHA GOD I LOVE THIS GOD FORSAKEN PROJECT

bool	is_in_env(char *to_add, char ***env)
{
	int	i;
	char	*temp1 = NULL;
	char	*temp2 = NULL;

	if (!to_add || !env)
		return false;
	i = 0;
	while ((*env)[i])
	{
		temp1 = ft_strndup(to_add, len_to_equal(to_add));
		temp2 = ft_strndup((*env)[i], len_to_equal((*env)[i]));
		if (ft_strcmp(temp1, temp2) == 0)
		{
			(*env)[i] = ft_strdup(to_add);
			free (temp1);
			free (temp2);
			return (true);
		}
		free (temp1);
		free (temp2);
		i++;
	}
	return (false);
}

int	declare_temp_vars(t_jobs *job, char ***env, char ***temp_vars)
{	
	int temp_vars_len = ft_split_wordcount(*temp_vars);
	int job_len = ft_split_wordcount(job->job);
	int	total_len = job_len + temp_vars_len;
	char **new_temp_vars = ft_calloc(sizeof(char *), total_len + 1);
	if (!new_temp_vars)
		return (0);
	int i = 0;
	while (i < temp_vars_len)
	{
		new_temp_vars[i] = ft_strdup(*temp_vars[i]);//error check
		i++;
	}
	int j = 0;
	while (j < job_len)
	{
		if (!is_in_env(job->job[j], env))//error check
		{
			//not working, apparently env is uninitialized?
			if (!is_in_env(job->job[j], &new_temp_vars))//error check
				new_temp_vars[i] = ft_strdup(job->job[j]);//error check
		}
		j++;
		i++;
	}
	new_temp_vars[total_len] = NULL;
	*temp_vars = new_temp_vars;
	return (0);
}

void	export_new(char *var, char **env)
{
	
}

//rename variables for better reading, error check for everything
void	export_temp(char *var, char ***env, char ***temp_vars)
{
  	int i = 0;
	char *temp;
	while ((*env)[i])
		i++;
/*	char *temp = ft_getenv(var, (*temp_vars));//gets temp env var value
	var = ft_strjoin(var, "=");//joins = sign
	temp = ft_strjoin(var, temp);//joins var name and tempenv var value
	(*env)[i] = ft_strdup(temp); */
	var = ft_strjoin(var, "=");//use vars[ft_strlen]
	temp = ft_strjoin(var, ft_getenv(var, *temp_vars));
	if (!is_in_env(temp, env))//error check
		(*env)[i] = ft_strdup(temp);//error check
}
// TODO NOT WORKING
int	caught_export(t_jobs *job, char ***env, char ***temp_vars)
{
/* 	int env_len = ft_split_wordcount(*env);
	int job_len = ft_split_wordcount(job->job);
	char **new_env = ft_calloc(sizeof(char *), job_len + env_len + 1);
	if (!new_env)
		return 0; */
	
	if (!job->job[1])
		return (caught_env(job, env));
	int i = 1;
	char	*temp = NULL;
	while (job->job[i])
	{
		if (ft_strchr(job->job[i], '='))//pass &env
		{
//			export_new(job->job[i], env);//error check
		}
		else
		{
			export_temp(job->job[i], env, temp_vars);//error check

/* 			//not
			job->job[i] = ft_strjoin(job->job[i], "=");//use vars[ft_strlen]
			temp = ft_strjoin(job->job[i], ft_getenv(job->job[i], *temp_vars));
			if (!is_in_env(temp, env))//error check
				(*env)[i] = ft_strdup(temp);//error check */
		}
		i++;
	}
	if ((*env)[32])
		printf("inside: %s\n", (*env)[32]);
	exit (0);//env doesnt save
}
/* 
int	caught_export_old(t_jobs *job, char ***env, char ***temp_vars)
{//take care of env-i
	char	*temp = NULL;
	int		i;
	int		j;

	i = 0;
	while ((*env)[i])
		i++;
	j = 0;
	while (job->job[j])
	{
		job->job[j] = ft_strjoin(job->job[j], "=");//use vars[ft_strlen]
		temp = ft_strjoin(job->job[j], ft_getenv(job->job[j], *temp_vars));
		if (!is_in_env(temp, env))//error check
			(*env)[i] = ft_strdup(temp);//error check
		j++;
		i++;
	}
	(*env)[i] = NULL;
	//free stuff
	
	return (0);
}
 */
int	try_builtins(t_jobs *job, char ***env, char ***temp_vars)
{
	if (ft_strcmp(job->job[0], "echo") == 0)
		return (caught_echo(job));
	else if (ft_strcmp(job->job[0], "env") == 0 
	|| (ft_strcmp(job->job[0], "export") == 0 && !job->job[1]))
		return (caught_env(job, env));
	else if (ft_strcmp(job->job[0], "pwd") == 0)
		return (caught_pwd(job, env));
	else if (ft_strcmp(job->job[0], "export") == 0)//"hello=world && export hello=mi", which stays?
		return (caught_export(job, env, temp_vars));//take care of env-i
	else if (ft_strcmp(job->job[0], "unset") == 0)//has to unset temp_vars as well
		return (caught_unset(job, env, temp_vars));
	else if (ft_strchr(job->job[0], '='))
		return (declare_temp_vars(job, env, temp_vars));
	return (200);//check if builtins fail, everything goes correct
}

// TODO has to unset temp_vars
int	unset_aux(char **to_remove, char ***env)
{
	int		i;
	while (*to_remove)
	{
		i = 0;
		while ((*env)[i] && ft_strncmp((*env)[i], *to_remove, ft_strlen(*to_remove)))
			i++;
		if (!(*env)[i])
			return (1);
		while ((*env)[i])
		{
			(*env)[i] = (*env)[i + 1];
			i++;
		}
		to_remove++;
	}
	return (0);
}

int	caught_unset(t_jobs *job, char ***env, char ***temp_vars)//segfault if var doesnt exist
{
	int		i;
	char	**to_remove;

	to_remove = ft_split(job->job[1], ' ');//error check
	if (unset_aux(to_remove, env))
		unset_aux(to_remove, temp_vars);
	exit (0);
}
// cd
void	cd_update_aux1(char **env, bool when)
{
	char	cwd[PATH_MAX];
	char	*temp;
	int	i;
	
	i = 0;
	if (when == BEFORE)
	{
		while (env[i] && ft_strncmp(env[i], "OLDPWD", 6))//fix oldpwd when cd fails
			i++;
		getcwd(cwd, PATH_MAX);//error check
		temp = ft_strjoin("OLDPWD=", cwd);//error check
		env[i] = temp;
	}
	else if (when == AFTER)
	{

		while (env[i] && ft_strncmp(env[i], "PWD", 3))
			i++;
		getcwd(cwd, PATH_MAX);//error check
		temp = ft_strjoin("PWD=", cwd);//error check
		env[i] = temp;
	}
}

// TODO cd can happen with just ..
// * check if just **env works, should because it doesnt exit process
int	caught_cd(t_jobs *job, char **env)//cd supposedly cant exit process, needs to be called before fork
{//check return values
	char 	*directory;
	char	*error;
	
	cd_update_aux1(env, BEFORE);
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
	cd_update_aux1(env, AFTER);
	return (0);
}
//env
int	caught_env(t_jobs *job, char ***env)//make better
{//env SHELL var has to be different
	int	i;

	if (job->job[1])//error and exit code
	{
		ft_putendl_fd("minishell env doesnt take that many args\n", 2);//permission denied
		return (126);
	}
	i = 0;
	while ((*env)[i])
	{
		ft_putstr_fd((*env)[i], 1);
		ft_nl_fd(1);
		i++;
	}
	exit (0);
}
//echo
int	caught_echo(t_jobs *job)
{
	bool	nl;

	if (!job->job[1])
		return (ft_nl_fd(1), 0);
	nl = true;
	if (ft_strncmp(job->job[0], "echo -n ", 8) == 0)
		nl = false;
	else if (ft_strncmp(job->job[0], "echo -n", 7) == 0)
		ft_printf("%s", job->job[0] + 5);
	else
		ft_printf("%s", job->job[1]);
	if (nl == true)
		ft_nl_fd(1);
	exit (0);
}

int	caught_pwd(t_jobs *job, char ***env)//make better
{
	char buf[PATH_MAX];

	if (job->job[1])//error and exit code
	{
		ft_putendl_fd("pwd: too many arguments", 2);
		return (1);
	}
	ft_putendl_fd(getcwd(buf, PATH_MAX), 2);//error check?
	exit (0);
}

void	check_exit(char *line)// wrong for job[0]1 | exit
{
	if (ft_strcmp(line, "exit") == 0)
	{
		free(line);
		rl_clear_history();
		exit(0);
	}
}
