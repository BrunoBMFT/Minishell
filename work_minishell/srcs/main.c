/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 16:43:23 by ycantin           #+#    #+#             */
/*   Updated: 2024/11/11 21:22:15 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int ac, char **av, char **envp)
{
	t_env	env;
	t_jobs	*jobs;
	char	*line;

	(void)av;
	if (ac != 1)
		return (1);
	env = init_env(envp);
	while (1)
	{
		choose_sig(ROOT_SIG);
		line = readline("Minishell$ ");
		if (!line)
			EOF_sig(line, &env);
		if (line && line[0])//remove if statement
			add_history(line);
		if (secondquote(line) == 1)	//remove if you want to request additional info to finish prompt
		{
			free(line);
			ft_printf("error: unclosed quote\n");
			continue ;
		}
		line = parse_quotes(line);//not working correctly?
		jobs = build(line, &env);
		start_executor(jobs, &env);
		clear_jobs(&jobs);
	}
	return (0);
}
