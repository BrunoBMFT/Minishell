/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 16:43:23 by ycantin           #+#    #+#             */
/*   Updated: 2024/09/03 22:43:04 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_env	init_env(char **envp)
{
	t_env	env;

	env.temp_vars = NULL;
	env.temp_vars_build = NULL;
	env.prompt = NULL;
	env.status = 0;
	env.env = dup_env(envp);
	return (env);
}

int	main(int ac, char **av, char **envp)
{
	t_env	env;
	char	*line;
	char	*dir;
	t_jobs	*jobs;
	t_jobs	*curr;

	env = init_env(envp);
	while (1)
	{
		signal(SIGINT, handle_signal_main);
		signal(SIGQUIT, SIG_IGN);
		env.prompt = update_prompt();
		line = readline(env.prompt);
		free(env.prompt);
		if (!line || !line[0])
		{
			free (line);
			continue ;
		}
		if (secondquote(line) == 1)	//remove if you want to request additional info to finish prompt
		{
			free(line);
			ft_printf("error: unclosed quote\n");
			continue ;
		}
		add_history(line);
		line = parse_quotes(line);
		jobs = build(line, env);
		curr = jobs;
		env.temp_vars = &env.temp_vars_build;
		env.status = start_executor(curr, env);
		clear_jobs(&jobs);
	}
	return (0);
}
