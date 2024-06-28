/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 07:26:16 by yohan             #+#    #+#             */
/*   Updated: 2024/04/24 18:23:51 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putchar_fd(char c, int fd)
{
	if (fd < 0)
		return ;
	write(fd, &c, 1);
}
/*
int main (void)
{
	int	fd;

	fd = open("/Users/yohan/Desktop/42 LISBOA/42_common_core/ft_putchar.c",
		O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror("open");
		return (1);
	}
		ft_putchar_fd('c', fd);
	close (fd);
	return (0);
}
*/