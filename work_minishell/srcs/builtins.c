/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 18:15:45 by bruno             #+#    #+#             */
/*   Updated: 2024/07/27 20:59:16 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
//check env vars problems (cd includesdadasd OR unset PATHASDHASDHASD)

int	caught_export(t_jobs *job, char **env, char **temp_vars)
{//take care of env-i
	char	**vars = NULL;
	char	*temp = NULL;
	char	**new_env = NULL;
	int		i;
	int		j;

	if (ft_strnstr(job->execd, "=", ft_strlen(job->execd)))
	{
		if (!job->execd)
			return (caught_env(job, env));
/* 		i = 0;
		while (job->job[i + 1])//shift job to skip export
		{
			job->job[i] = job->job[i + 1];
			i++;
		}
		job->job[i] = job->job[i + 1];//shift */
/* 		i = 0;
		while (job->job[i])
		{
			printf("%d: %s\n", i, job->job[i]);
			i++;
		} */
		vars = ft_split(job->job[1], ' ');//error check
		new_env = add_to_env(vars, env);//errorcheck and free
		i = 0;
		while (new_env[i])
		{
			env[i] = ft_strdup(new_env[i]);//errorcheck and free
			i++;
		}
		env[i] = NULL;
/* 		i = 0;
		while (env[i])
		{
			if (new_env[i])
				printf("new_env: %d: %s\n", i, new_env[i]);
			if (env[i])
				printf("env: %d: %s\n", i, env[i]);
			i++;
		} */
/* 		printf("env: %d\n", ft_split_wordcount(env));
		printf("new_env: %d\n", ft_split_wordcount(new_env)); */
		//free stuff
	}
	else
	{
		vars = ft_split(job->execd, ' ');//error check
		i = 0;
		while (env[i])
			i++;
		j = 0;
		while (vars[j])
		{
			temp = ft_getenv(vars[j], temp_vars);//error check
			vars[j] = ft_strjoin(vars[j], "=");
			temp = ft_strjoin(vars[j], temp);
			env[i] = ft_strdup(temp);//error check
			j++;
			i++;
		}
		env[i] = NULL;
		//free stuff
	}
	
	return (0);
}

int	try_builtins(t_jobs *job, char **env, char **temp_vars)
{
	if (ft_strcmp(job->cmd, "cd") == 0)
		return (caught_cd(job, env));
	if (ft_strcmp(job->cmd, "echo") == 0)
		return (caught_echo(job));
	else if (ft_strcmp(job->cmd, "env") == 0)
		return (caught_env(job, env));
	else if (ft_strcmp(job->cmd, "pwd") == 0)
		return (caught_pwd(job, env));
	else if (ft_strcmp(job->cmd, "unset") == 0)//has to unset temp_vars as well
		return (caught_unset(job, env, temp_vars));
	else if (ft_strcmp(job->cmd, "export") == 0)//"hello=world && export hello=mi", which stays?
		return (caught_export(job, env, temp_vars));
	else if (ft_strcmp(job->cmd, "clear") == 0)// * REMOVE
		return (clear_proc(env));// * REMOVE
	return (200);
}
//has to unset temp_vars
int	unset_aux(char **to_remove, char **env)
{
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

int	caught_unset(t_jobs *job, char **env, char **temp_vars)//segfault if var doesnt exist
{
	int		i;
	char	**to_remove;

	to_remove = ft_split(job->execd, ' ');//error check
	if (unset_aux(to_remove, env))
		unset_aux(to_remove, temp_vars);
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
int	caught_env(t_jobs *job, char **env)//make better
{
	int	i;

	if (job->execd)//error and exit code
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
int	caught_pwd(t_jobs *job, char **env)//make better
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
int	caught_echo(t_jobs *job)
{
	bool	nl;

	if (!job->execd)
		return (ft_nl_fd(1), 0);
	nl = true;
	if (ft_strncmp(job->cmd, "echo -n ", 8) == 0)
		nl = false;
	else if (ft_strncmp(job->cmd, "echo -n", 7) == 0)
		ft_printf("%s", job->cmd + 5);
	else
		ft_printf("%s", job->execd);
	if (nl == true)
		ft_nl_fd(1);
	return (0);
}
