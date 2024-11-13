/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 16:43:23 by ycantin           #+#    #+#             */
/*   Updated: 2024/11/13 17:33:38 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
void	print_jobs(char *line, t_jobs *jobs)
{
	ft_printf_fd(2, "line: %s\n", line);
	int i = 0;
	while (jobs->job[i])
	{
		ft_printf_fd(2, "job str %d: %s\n", i, jobs->job[i]);
		i++;
	}
}
void	minishell(char **envp)
{
	char	*line;
	t_env	env;
	t_jobs	*jobs;
	t_jobs	*current;

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
		jobs = build(line, &env);
		current = jobs;
		start_executor(current, &env);
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
