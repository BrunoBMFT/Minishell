/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 16:43:23 by ycantin           #+#    #+#             */
/*   Updated: 2024/08/31 17:44:45 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**dup_env(char **envp)
{
	char	**new_env;
	int		i;

	if (!envp || !envp[0])
		return (NULL);//printf something
	new_env = ft_calloc(sizeof(char *), ft_split_wordcount(envp) + 2);
	if (!new_env)
		return (NULL);//free new_env?
	i = 0;
	while (envp[i] && envp[i])
	{
		if (ft_strncmp(envp[i], "SHLVL=", 6) == 0)//check size
			new_env[i] = ft_strjoin("SHLVL=", ft_itoa(ft_atoi(envp[i] + 6) + 1));//error check
		else
			new_env[i] = ft_strdup(envp[i]);//error check
		if (!new_env[i])
		{
			i = 0;
			while (new_env[i])
				free (new_env[i++]);//check if works
			return (NULL);
		}
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

int	main(int ac, char **av, char **envp)
{
	char	**env = dup_env(envp);
	char	*line;
	char	*dir;
	char	*prompt;
	t_jobs	*jobs;
	t_jobs	*curr;
	char	**temp_vars = NULL;
	int		status = 0;

	while (1)
	{
		prompt = update_prompt();
/* 		if (set_signal(SIGINT, ctrl_c_idle) < 0 || set_signal(SIGQUIT, SIG_IGN) < 0)
		{
			free(prompt);
			ft_printf("Error: signal\n");
			return 1;
		} */
		line = readline(prompt);
		free(prompt);
		if (!line || !line[0])
		{
			free (line);
			continue ;
		}
		add_history(line);
		line = parse_quotes(line);
		jobs = build(line, env, temp_vars, status);
		curr = jobs;
		status = start_executor(curr, env, &temp_vars);
		clear_jobs(&jobs);
	}
	return (0);
}
