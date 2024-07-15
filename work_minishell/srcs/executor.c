/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 17:26:33 by bruno             #+#    #+#             */
/*   Updated: 2024/07/15 17:25:27 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*find_path(char *command, char **env)
{
	//use strcmp
	char	**paths;
	char	*path;

	while (*env && ft_strnstr(*env, "PATH", 4) == 0)
		env++;
	if (!*env)
		return NULL;
	paths = ft_split(*env, ':');
	while (*paths)
	{
		*paths = ft_strjoin(*paths, "/");
		path = ft_strjoin(*paths, command);
		if (access(path, F_OK) == 0)
			return (path);
		free (path);
		paths++;
	}
	free_array(paths);
	return (NULL);
}

bool	execute_job(char **command, char **env)
{
	char	*path = find_path(command[0], env);
//	ft_putstr_fd("command: ", 2);
//	ft_putstr_fd(command[0], 2);
//	ft_putstr_fd("\n", 2);
	if (!path)
		return (printf("no command\n"), false);//free the commands
	execve(path, command, env);
	printf("execve failed\n");
	free_array(command);
	return true;
}

int	child_process(t_jobs *job, char **env)
{
	int		fd[2];
	int		status = 0;

	pipe(fd);
	pid_t pid = new_fork();
	if (pid == 0)
	{
		close(fd[READ]);
		dup2(fd[WRITE], STDOUT_FILENO);
		close(fd[WRITE]);
		if (!execute_job(job->job, env))
			panic("execute");
	}
	close(fd[WRITE]);
	dup2(fd[READ], STDIN_FILENO);
	close(fd[READ]);
	waitpid(pid, &status, 0);
	return WEXITSTATUS(status);
}

int	simple_process(t_jobs *job, char **env)
{
	int	pid = new_fork();
	if (pid == 0)
	{
		if (!execute_job(job->job, env))
			panic("simple execute failed\n");//free fds, show exit code and perror
	}
	int status = 0;
	waitpid(pid, &status, 0);
//	printf("status: %d\n", WEXITSTATUS(status));
	return (WEXITSTATUS(status));
}
// TODO
/* struct cmd *pipecmd(struct cmd *left, struct cmd *right)
{
	struct pipecmd *cmd;

	cmd = malloc(sizeof(*cmd));
	memset(cmd, 0, sizeof(*cmd));
	cmd->type = PIPE;
	cmd->left = left;
	cmd->right = right;
	return (struct cmd *)cmd;
} */

void	run_execution(t_jobs *curr, char **env)//
{
	int status = 0;
	while (curr && curr->next)
	{
//		printf("cmd: %s\t  execd: %s\t  type:%d\n", curr->cmd, curr->execd, curr->type);
		if (curr->next->type == 1)//(|)
		{
			child_process(curr, env);
			curr = curr->next;
		}
		if (curr->next->type == 2)//(&&)
		{
			simple_process(curr, env);
			curr = curr->next;
		}
/* 		if (curr->next->type == 3)//(||) not working for now cause of last process running
		{
			if (simple_process(curr, env) != 0)
			{
				curr = curr->next;
				simple_process(curr, env);
			}
		} */
//		child_process(curr, env);
		curr = curr->next;
	}
	status = simple_process(curr, env);
//	printf("final status: %d\n", status);
}
