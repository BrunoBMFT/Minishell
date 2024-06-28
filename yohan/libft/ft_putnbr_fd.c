/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 19:59:45 by yohan             #+#    #+#             */
/*   Updated: 2024/04/24 18:23:58 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	if (fd < 0)
		return ;
	if (n == -2147483648)
		write(fd, "-2147483648", 11);
	else if (n < 0)
	{
		write(fd, "-", 1);
		ft_putnbr_fd(-n, fd);
	}
	else if (n > 9)
	{
		ft_putnbr_fd(n / 10, fd);
		ft_putnbr_fd(n % 10, fd);
	}
	else
	{
		n += 48;
		write(fd, &n, 1);
	}
}

/*
int main (void)
{
	int	fd;
	int	num;

	fd = open("ft_putnbr.c", O_WRONLY | O_CREAT | O_APPEND , 0644);
	if (fd == -1)
	{
		perror ("open");
		return (1);
	}
	num = 0;
	ft_putnbr_fd(num, fd);
	write (fd, "\n", 1);
	return (0);
}
*/
