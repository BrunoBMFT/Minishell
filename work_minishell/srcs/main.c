/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 16:43:23 by ycantin           #+#    #+#             */
/*   Updated: 2024/07/27 20:59:06 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
//change strncmp for strcmp
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
//		if (set_up_signal(ctrl_c_idle) < 0)
//			clean_exit(jobs, line, prompt);
		line = readline(prompt);
		free(prompt);
		if (!line || !line[0])
			continue ;
		add_history(line);
		check_exit(line);
		line = expand_env_vars(line, env, temp_vars);
		jobs = build(line);
		if (ft_strnstr(jobs->cmd, "=", ft_strlen(jobs->cmd)))
			temp_vars = add_to_env(jobs->job, temp_vars);
/* 		int i = 0;
 		if (temp_vars)
		{
			
			while(temp_vars[i])
			{
				printf("%d: %s\n", i, temp_vars[i]);
				i++;
			}
		} */
		start_executor(jobs, env, temp_vars);
		clear_jobs(&jobs);
	}
	return (0);
}
