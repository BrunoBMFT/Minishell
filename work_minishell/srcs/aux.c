/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 19:15:54 by bruno             #+#    #+#             */
/*   Updated: 2024/07/19 23:41:47 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	panic(char *s)
{
	ft_putendl_fd(s, 2);
	exit(1);
}

int	new_fork(void)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		panic("fork");
	return (pid);
}


char	*update_prompt()
{
	char	cwd[100];
	char	*dir;
	char	*prompt;
	char	**folders;
	int		i;
	
	dir = getcwd(cwd, sizeof(cwd));//error check
	folders = ft_split(dir, '/');//error check
	i = 0;
	while (folders[i])
		i++;
	prompt = folders[i - 1];
	prompt = ft_strjoin(prompt, "$ ");//error check
	free_array(folders);
	return (prompt);
}

t_jobs *build(char *command_line)
{
	t_jobs *jobs;
	t_token *list;

	list = NULL;
	jobs = NULL;
	tokenize(&list, command_line);
	make_job_list(&jobs, &list);
	clear_list(&list);
	return (jobs);
}

int	clear_proc(char **env)// * REMOVE
{
	int	pid = new_fork();
 	if (pid == 0)
	{
		char *const com[] = { "clear", NULL };
		execve("/bin/clear", com, env);
		panic("clear fail\n");//free fds, show exit code and perror
	}
	int status = 0;
	return (0);
}

char	*expand_env_vars(char *input, char **env)
{
	int 	i;//use pointers instead?
	bool	flag = false;//flag is to skip the case which it finds a env variable when it wasnt declared (USER without the $ before)
	char	**vars = ft_split(input, '$');//error check
	char	*output = NULL;
	char	*temp;

	if (input[0] != '$')
		flag = true;
	while (*vars)//running through each word
	{
		if (flag)
			flag = false;// dont hardcode like this
		else
		{
			i = 0;
			while (env[i])
			{
				temp = ft_strndup(env[i], len_to_equal(env[i]));
				if (ft_strncmp(temp, *vars, ft_strlen(temp)) == 0)
					break;
				i++;
			}
			if (!env[i])
				return (NULL);//print new line
			*vars = ft_env_var(env[i]);// error check, wrong size allocated
		}
		if (!output)
			output = ft_strdup(*vars);//error check
		else
			output = ft_strjoin(output, *vars);//error check
		vars++;
	}
	if (input[ft_strlen(input) - 1] == '$')//! dont hardcode like this
		ft_strcat(output, "$");
//	ft_split_free(vars);
	return (output);
}