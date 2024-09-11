/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brfernan <brfernan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 16:43:23 by ycantin           #+#    #+#             */
/*   Updated: 2024/09/11 16:52:54 by brfernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
int	main(int ac, char **av, char **envp)
{
	t_env	env;
	t_jobs	*jobs;
	t_jobs	*curr;
	char	*line;
	char	*dir;

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
		printf("line: %s\n", line);
		jobs = build(line, env);
		curr = jobs;
		
		int i = 0;
		while (jobs->job[i])
		{
			printf("job %d: %s\n", i, jobs->job[i]);
			i++;
		}

		env.status = start_executor(curr, env);
		clear_jobs(&jobs);
	}
	return (0);
}
