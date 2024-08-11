/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 16:43:23 by ycantin           #+#    #+#             */
/*   Updated: 2024/08/10 20:11:18 by bruno            ###   ########.fr       */
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
/* 		
line = expand_env_vars(line, env, temp_vars);
		if (ft_strnstr(line, "=", ft_strlen(line)))
			temp_vars = vars_declaration(line, temp_vars, env);
		if (ft_strnstr(line, "=", ft_strlen(line)))
		{
			temp_vars = vars_declaration(line, temp_vars, env);//needs to be taken care of in process so exit code doesnt change
			if (temp_vars)
			{
				int i = 0;
				while (temp_vars[i])
				{
					printf("%d: %s\n", i, temp_vars[i]);
					i++;
				}
			}
		}
		if (ft_strncmp(line, "env", 3) == 0)
			printenv(env); */
		jobs = build(line, env);
		curr = jobs;
		start_executor(jobs, env, temp_vars);
//		clear_jobs(&jobs);
	}
	return (0);
}
