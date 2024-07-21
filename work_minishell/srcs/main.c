/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 16:43:23 by ycantin           #+#    #+#             */
/*   Updated: 2024/07/21 17:20:20 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	check_exit(char *line)// wrong for cmd1 | exit
{
	if (ft_strcmp(line, "exit") == 0)
	{
		free(line);
//		rl_clear_history();
		exit(0);
	}
}


int main (int ac, char **av, char **envp)
{
	char	**env = envp;
	char	*line;
	char	*dir;
	char	*prompt;
	t_jobs	*jobs;
	char	**temp_vars = NULL;
	int i = 0;
	while (1)
	{
		prompt = update_prompt();
//		if (set_up_signal(ctrl_c_idle) < 0)
//			clean_exit(jobs, line, prompt);
		line = readline(prompt);
		free(prompt);
//*		line = expand_env_vars(line, env);//has to expand temp_vars
//		check_exit(line);
		add_history(line);
		i++;
		printf("job: %d\n", i);
		jobs = build(line);
//*		if (ft_strnstr(jobs->cmd, "=", ft_strlen(jobs->cmd)) && !jobs->execd)//cmd: "export=" doesnt export or save anything
//*			temp_vars = variable_declaration(jobs->cmd, temp_vars);
		start_executor(jobs, env);
//		free(prompt);
		clear_jobs(&jobs);//edited by bruno
	}
	return (0);
}
