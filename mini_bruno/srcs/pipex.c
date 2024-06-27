/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 18:07:23 by bruno             #+#    #+#             */
/*   Updated: 2024/06/17 18:08:42 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	close_fds_exit(int *fd, char *str)
{
	close(fd[0]);
	close(fd[1]);
	error(str, 1);
}

void	error(char *str, int code)
{
	write(2, "bash: ", 6);
	perror(str);
	exit(code);
}

void	error2(char *str, int code, int *fd)
{
	char	**new;

	close(fd[0]);
	close(fd[1]);
	new = ft_split(str, ' ');
	write(2, new[0], ft_strlen(new[0]));
	write(2, ": command not found\n", 20);
	freecoms(new);
	exit(code);
}

bool	path_exists(char **envp)
{
	int		i;

	i = 0;
	while (envp[i])
	{
		if (ft_strnstr(envp[i], "PATH", 4) != 0)
			return (true);
		i++;
	}
	return (false);
}

char	*find_path(char **envp, char *com)
{
	char	*path;
	char	*part;
	char	**paths;
	int		i;

	i = 0;
	if (!path_exists(envp))
		return (NULL);
	if (access(com, F_OK) == 0)
		return (com);
	while (ft_strnstr(envp[i], "PATH", 4) == 0)
		i++;
	paths = ft_split(envp[i] + 5, ':');
	i = -1;
	while (paths[++i])
	{
		part = ft_strjoin(paths[i], "/");
		path = ft_strjoin(part, com);
		free (part);
		if (access(path, F_OK) == 0)
			return (path);
		free (path);
	}
	freecoms(paths);
	return (NULL);
}

void	freecoms(char **cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		free(cmd[i]);
		i++;
	}
	free (cmd);
}

bool	execute(char *arg, char **envp)
{
	char	**com;
	char	*path;

	com = ft_split(arg, ' ');
	path = find_path(envp, com[0]);
	if (!path)
		return (freecoms(com), false);
	execve(path, com, envp);
	freecoms(com);
	return (false);
}

void	child1_process(int *fd, char **av, char **envp)
{
	int	filein;

	filein = open(av[1], O_RDONLY, 0644);
	if (filein == -1)
		close_fds_exit(fd, av[1]);
	close(fd[READ]);
	dup2(filein, STDIN_FILENO);
	close(filein);
	dup2(fd[WRITE], STDOUT_FILENO);
	close(fd[WRITE]);
	if (!av[2][0] || av[2][0] == ' ' || !execute(av[2], envp))
		error2(av[2], 1, fd);
}

void	child2_process(int *fd, char **av, char **envp)
{
	int	fileout;

	fileout = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fileout == -1)
		close_fds_exit(fd, av[4]);
	close(fd[WRITE]);
	dup2(fd[READ], STDIN_FILENO);
	close(fd[READ]);
	dup2(fileout, STDOUT_FILENO);
	close(fileout);
	if (!av[3][0] || av[3][0] == ' ')
		error2(av[3], 126, fd);
	if (!execute(av[3], envp))
		error2(av[3], 127, fd);
}
/*
int	main(int ac, char **av, char **envp)
{
	int		fd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status;

	status = 0;
	if (ac != 5)
		return (ft_putendl_fd("wrong: ./pipex <file1> <cmd1> <cmd2> <file2>", 2), 2);
	if (pipe(fd) == -1)
		error("pipe failed", 1);
	pid1 = fork();
	if (pid1 < 0)
		error("pid1 error", 0);
	else if (pid1 == 0)
		child1_process(fd, av, envp);
	pid2 = fork();
	if (pid2 < 0)
		error("pid2 error", 0);
	else if (pid2 == 0)
		child2_process(fd, av, envp);
	close(fd[WRITE]);
	close(fd[READ]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, &status, 0);
	return (WEXITSTATUS(status));
}*/
