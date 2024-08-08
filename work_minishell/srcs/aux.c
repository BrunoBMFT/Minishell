/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 19:15:54 by bruno             #+#    #+#             */
/*   Updated: 2024/08/08 23:13:00 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void print_jobs(t_jobs *jobs) 
{
    t_jobs *curr = jobs;
    while (curr != NULL) 
	{
        int i = 0;
        printf("cmd: %s  execd: %s  type: %d\n", curr->cmd, curr->execd, curr->type);
/*         while (curr->job && curr->job[i]) 
		{
            printf("job: %s\n", curr->job[i]);
            i++;
        } */
        curr = curr->next;
    }
}

void	check_exit(char *line)// wrong for cmd1 | exit
{
	if (ft_strcmp(line, "exit") == 0)
	{
		free(line);
		rl_clear_history();
		exit(0);
	}
}

void	panic(char *s)
{
	ft_putendl_fd(s, 2);
	exit(1);
}

int	new_fork(void)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		panic("fork");
	return (pid);
}

char	*update_prompt()
{
	char	cwd[100];
	char	*dir;
	char	*prompt;
	char	**folders;
	int		i;
	
	dir = getcwd(cwd, sizeof(cwd));//error check
	folders = ft_split(dir, '/');//error check
	i = 0;
	while (folders[i])
		i++;
	prompt = folders[i - 1];
	prompt = ft_strjoin(prompt, "$ ");//error check
	free_array(folders);
	return (prompt);
}

void	clean_up_build(t_token **list, char *cmd_line)
{
	clear_list(list);
	free(cmd_line);
}

int parse_last_token(char **cmd_line, t_token **list, t_token **last)
{
	char *line;
	char *new;
	char *converted;

	line = readline("> ");
	if (!line)
	{
		clean_up_build(list, *cmd_line);
		return (-1);
	}
	new = ft_strjoin(*cmd_line, line);
	free(*cmd_line);
	free(line);
	*cmd_line = new;
	clear_list(list);
	converted = split_complex_args(*cmd_line);
	tokenize(list, converted);
	free(converted);
	*last = get_last_tok(*list);
	if (parse(list) == -1)
	{
		clean_up_build(list, *cmd_line);
		return (-1);
	}
	return (0);
}

t_jobs *build(char *command_line)
{
    t_jobs *jobs;
    t_token *list;
    t_token *last;
    char *converted;

	jobs = NULL;
	list  = NULL;
	last = NULL;
    converted = split_complex_args(command_line);
    tokenize(&list, converted);
    free(converted);
    last = get_last_tok(list);
    if (parse(&list) == -1) 
	{
        clear_list(&list);
        return (NULL);
    }
    while (last && last->type >= PIPE && last->type <= OR && !last->next)
		if (parse_last_token(&command_line, &list, &last) == -1)
			return (NULL);
    make_job_list(&jobs, &list);
    clean_up_build(&list, command_line);
    return (jobs); 
}

int	clear_proc(char **env)// * REMOVE
{
	int	pid = new_fork();
 	if (pid == 0)
	{
		char *const com[] = { "clear", NULL };
		execve("/bin/clear", com, env);
		panic("clear fail\n");//free fds, show exit code and perror
	}
	int status = 0;
	return (0);
}
