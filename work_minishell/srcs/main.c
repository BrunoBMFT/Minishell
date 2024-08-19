/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 16:43:23 by ycantin           #+#    #+#             */
/*   Updated: 2024/08/19 01:58:38 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int ac, char **av, char **envp)
{
	char	**env = envp;
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
/* 		
if (set_signal(SIGINT, ctrl_c_idle) < 0 || set_signal(SIGQUIT, SIG_IGN) < 0)
			clean_exit(jobs, line, prompt); */
		line = readline(prompt);
		free(prompt);
		if (!line || !line[0])
			continue ;//free line?
		line = parse_quotes(line);
/* 		
if (secondquote(line))
		{
			free(line);
			ft_printf("error: unclosed quote\n");
			continue ;
		} */
		check_exit(line);//technically a builtin
		add_history(line);
		jobs = build(line, env, status);//find better way to send status
		curr = jobs;
		status = start_executor(curr, env, &temp_vars);
/* 		int i = 0;
		while (temp_vars && temp_vars[i])
		{
			printf("temp_vars %d: %s\n", i, temp_vars[i]);
			i++;
		} */
		clear_jobs(&jobs);
	}
	return (0);
}
