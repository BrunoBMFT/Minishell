/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 12:39:01 by ycantin           #+#    #+#             */
/*   Updated: 2024/04/30 14:45:41 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRINTF_H
# define PRINTF_H

# include <stdarg.h>
# include <stddef.h>
# include <stdio.h>
# include <unistd.h>

void	ft_putnbr(int nbr, int *len);
void	ft_lowerhex(unsigned int nbr, int *len);
void	ft_upperhex(unsigned int nbr, int *len);
void	ft_putdecvalue(unsigned int nbr, int *len);
void	ft_printwholeaddress(size_t pointer, int *len);
void	ft_putchar(char c, int *len);
void	ft_putstr(char *str, int *len);

void	ft_printcommands(char c, va_list *conversions, int *len, int *i);
int		ft_printf(const char *str, ...);

#endif