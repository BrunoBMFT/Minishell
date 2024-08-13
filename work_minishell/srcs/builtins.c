/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 18:15:45 by bruno             #+#    #+#             */
/*   Updated: 2024/08/13 03:35:44 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// ! AFTER UNSETTING PWD, THE PWD COMMAND HAS TO STILL WORK
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

int	try_builtins(t_jobs *job, char **env, char **temp_vars)
{
	printf("builtins\n");
	if (ft_strcmp(job->job[0], "echo") == 0)
		return (caught_echo(job));
	else if (ft_strcmp(job->job[0], "env") == 0)
		return (caught_env(job, env));
	else if (ft_strcmp(job->job[0], "pwd") == 0)
		return (caught_pwd(job, env));
	else if (ft_strcmp(job->job[0], "unset") == 0)//has to unset temp_vars as well
		return (caught_unset(job, env, temp_vars));
/* 	else if (ft_strcmp(job->job[0], "export") == 0)//"hello=world && export hello=mi", which stays?
		return (caught_export(job, env, temp_vars)); */
	return (200);//check if builtins fail, everything goes correct
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

	to_remove = ft_split(job->job[1], ' ');//error check
	if (unset_aux(to_remove, env))
		unset_aux(to_remove, temp_vars);
	return (0);
}
// cd 
// cd can happen with just ..
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


void	check_exit(char *line)// wrong for job[0]1 | exit
{
	if (ft_strcmp(line, "exit") == 0)
	{
		free(line);
		rl_clear_history();
		exit(0);
	}
}

int	caught_export(t_jobs *job, char **env, char **temp_vars)
{//take care of env-i
/* 	char	**vars = NULL;
	char	*temp = NULL;
	char	**new_env = NULL;
	int		i;
	int		j;

	if (ft_strnstr(job->job[1], "=", ft_strlen(job->job[1])))
	{
		if (!job->job[1])
			return (caught_env(job, env));
		vars = ft_split(job->job[1], ' ');//error check
		new_env = add_to_env(vars, env);//errorcheck and free
		i = 0;
		while (new_env[i])
		{
			env[i] = ft_strdup(new_env[i]);//errorcheck and free
			i++;
		}
		env[i] = NULL;
		//free stuff
	}
	else
	{
		vars = ft_split(job->job[1], ' ');//error check
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
	 */
	return (0);
}