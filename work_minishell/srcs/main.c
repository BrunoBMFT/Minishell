/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 16:43:23 by ycantin           #+#    #+#             */
/*   Updated: 2024/11/01 02:53:44 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int ac, char **av, char **envp)
{
	t_env	env;
	t_jobs	*jobs;
	t_jobs	*curr;
	char	*line;

	(void)av;
	if (ac != 1)
		return (1);
	env = init_env(envp);
	while (1)
	{
		choose_sig(ROOT_SIG);
//		line = readline("Minishell> ");
		env.prompt = update_prompt();//to remove
		line = readline(env.prompt);//to remove
		free(env.prompt);//to remove
		if (!line)
			ctrld(line, &env);
		if (line && line[0])
			add_history(line);
		if (secondquote(line) == 1)	//remove if you want to request additional info to finish prompt
		{
			free(line);
			ft_printf("error: unclosed quote\n");
			continue ;
		}
		line = parse_quotes(line);//not working correctly?
		jobs = build(line, &env);
		if (!jobs)//print job error?
			continue ;
		curr = jobs;//why use curr?
		start_executor(curr, &env);
		clear_jobs(&jobs);
	}
	return (0);
}
