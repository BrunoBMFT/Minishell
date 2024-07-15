/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 16:43:23 by ycantin           #+#    #+#             */
/*   Updated: 2024/07/12 20:27:53 by bruno            ###   ########.fr       */
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
	t_jobs	*curr;
	while (1)
	{
		prompt = update_prompt();
		line = readline(prompt);
		free(prompt);
		check_exit(line);
		add_history(line);
		jobs = build(line);
		curr = jobs;
		run_execution(curr, env);
//		free(prompt);
		clear_jobs(&jobs);//edited by bruno
	}
	return (0);
}


/* 
int main(void)
{
	char *str = "cat file.txt | sort > output.txt && grep 'search' output.txt && echo done";
	t_jobs *jobs;
	jobs = build(str);
	t_jobs *currr = jobs;
	int i = 0;
	while (currr != NULL)
	{
		i++;
		printf("%s  %s\n", currr->cmd, currr->execd);
		currr = currr->next; 
	}
	clear_jobs(&jobs);
} */