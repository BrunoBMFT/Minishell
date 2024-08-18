/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 18:15:45 by bruno             #+#    #+#             */
/*   Updated: 2024/08/18 04:40:10 by bruno            ###   ########.fr       */
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
			return (true);
		}
		i++;
	}
	return (false);
}

int	declare_temp_vars(t_jobs *job, char **env, char ***temp_vars)
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
		if (!is_in_env(job->job[j], &env))//error check
		{
			//not working, apparently env is uninitialized?
			if (!is_in_env(job->job[j],&new_temp_vars))//error check
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
void	export_temp(char **new_env, char *var, char ***env, char ***temp_vars)
{
 	int i = 0;
	while ((*env)[i])// * here (can be in another function since export new will have to use it)
	{
		new_env[i] = ft_strdup((*env)[i]);//error check
		i++;
	}// * here
	char *temp = ft_strjoin(var, "=");//error check
	char *env_var = ft_strjoin(temp, ft_getenv(var, *temp_vars));//error check
	if (!is_in_env(env_var, env))//error check
		new_env[i] = ft_strdup(env_var);//error check

	new_env[i + 1] = NULL;
/* 	 i = 0;
	 ft_nl();
	while ((*env)[i])
	{
		printf("%s\n", (*env)[i]);
		i++;
	} */
	printf("last env var: %s\n", new_env[i]);
}

int	caught_export(t_jobs *job, char ***env, char ***temp_vars)
{
	int i = 1;
	int env_len = ft_split_wordcount(*env);
	int job_len = ft_split_wordcount(job->job);
	char **new_env = ft_calloc(sizeof(char *), job_len + env_len + 1);
	if (!new_env)
		return 0;
	
	while (job->job[i])
	{
		if (ft_strchr(job->job[i], '='))//pass &env
		{
//			export_new(job->job[i], env);//error check
		}
		else
		{
			export_temp(new_env, job->job[i], env, temp_vars);//error check
		}
		i++;
	}
	*env = new_env;
 	i = 0;
	while ((*env)[i + 1])
		i++;
	printf("last env var: %s\n", (*env)[i]);
	return (0);
}

int	try_builtins(t_jobs *job, char **env, char ***temp_vars)
{
	if (ft_strcmp(job->job[0], "echo") == 0)
		return (caught_echo(job));
	else if (ft_strcmp(job->job[0], "env") == 0)
		return (caught_env(job, env));
	else if (ft_strcmp(job->job[0], "pwd") == 0)
		return (caught_pwd(job, env));
	else if (ft_strcmp(job->job[0], "export") == 0)//"hello=world && export hello=mi", which stays?
		return (caught_export(job, &env, temp_vars));//take care of env-i
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

int	caught_unset(t_jobs *job, char **env, char ***temp_vars)//segfault if var doesnt exist
{
	int		i;
	char	**to_remove;

	to_remove = ft_split(job->job[1], ' ');//error check
	if (unset_aux(to_remove, &env))
		unset_aux(to_remove, temp_vars);
	return (0);
}
// cd 
// TODO cd can happen with just ..
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
int	caught_cd(t_jobs *job, char **env)//cd supposedly cant exit process, needs to be called before fork
{//check return values
	char 	*directory;
	
	cd_update_pwd(env, BEFORE);
	directory = job->job[1];
 	if (!job->job[1])
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
		ft_putstr_fd(env[i], 1);
		ft_nl_fd(1);
		i++;
	}
	return (0);
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

int	caught_pwd(t_jobs *job, char **env)//make better
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

void	check_exit(char *line)// wrong for job[0]1 | exit
{
	if (ft_strcmp(line, "exit") == 0)
	{
		free(line);
		rl_clear_history();
		exit(0);
	}
}
