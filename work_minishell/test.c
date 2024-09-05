/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 19:21:05 by brfernan          #+#    #+#             */
/*   Updated: 2024/09/04 22:20:20 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int	main(int ac, char **av)
{
	int	i;

	i = 1;
	while (i < ac)
	{
		ft_printf_fd(1, "%s\n", av[i]);
		i++;
	}
	return (0);
}

/* int	main(void)
{
	int	fd1;
	int	fd2;
	char *ret;

	ft_printf_fd(fd2, "here");
	fd1 = open("/home/bruno/work_minishell/test.c", O_RDONLY);
	fd2 = open("/home/bruno/work_minishell/WAWAWA", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd1 == -1)
		return (perror("Error in opening 1 file"), 1);
	if (fd2 == -1)
		return (perror("Error in opening 2 file"), 1);
	ret = get_next_line(fd1);
	while (ret)
	{
		ft_printf_fd(fd2, "hello: %s", ret);
		free(ret);
		ret = get_next_line(fd1);
	}
	close(fd1);
	close(fd2);
	return (0);
} */