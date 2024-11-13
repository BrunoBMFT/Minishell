/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 16:43:23 by ycantin           #+#    #+#             */
/*   Updated: 2024/11/11 21:28:51 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	minishell(char **envp)
{
	char	*line;
	t_env	env;
	t_jobs	*jobs;

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
}

int	main(int ac, char **av, char **envp)
{
	(void)av;
	if (ac != 1)
		return (1);
	minishell(envp);
	return (0);
}
