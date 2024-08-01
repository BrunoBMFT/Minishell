/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 16:43:23 by ycantin           #+#    #+#             */
/*   Updated: 2024/08/01 23:16:24 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int main (int ac, char **av, char **envp)
{
	char	**env = envp;
	char	*line;
	char	*dir;
	char	*prompt;
	t_jobs	*jobs;
	char	**temp_vars = NULL;

	while (1)
	{
		prompt = update_prompt();
/* 		if (set_signal(SIGINT, ctrl_c_idle) < 0 || set_signal(SIGQUIT, SIG_IGN) < 0)
			clean_exit(jobs, line, prompt); */
		line = readline(prompt);
		free(prompt);
		if (!line || !line[0])//find better way
			continue ;
		check_exit(line);
		add_history(line);
//		line = expand_env_vars(line, env, temp_vars);
		jobs = build(line);
//		temp_vars = variable_declaration(jobs, temp_vars, env);
		start_executor(jobs, env, temp_vars);
		clear_jobs(&jobs);
	}
	return (0);
}
