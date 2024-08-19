/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 20:54:30 by yohan             #+#    #+#             */
/*   Updated: 2024/07/09 16:18:59 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "printf.h"

void	ft_printcommands(char c, va_list *conversions, int *len, int *i)
{
	if (c == 'c')
		ft__printf_putchar(va_arg(*conversions, int), len);
	else if (c == 's')
		ft_printf_putstr(va_arg(*conversions, char *), len);
	else if (c == '%')
		ft__printf_putchar('%', len);
	else if (c == 'd' || c == 'i')
		ft_printf_putnbr(va_arg(*conversions, int), len);
	else if (c == 'x')
		ft_lowerhex(va_arg(*conversions, unsigned int), len);
	else if (c == 'X')
		ft_upperhex(va_arg(*conversions, unsigned int), len);
	else if (c == 'u')
		ft_putdecvalue(va_arg(*conversions, unsigned int), len);
	else if (c == 'p')
		ft_printwholeaddress(va_arg(*conversions, size_t), len);
	else
		(*i)--;
}

int	ft_printf(const char *str, ...)
{
	va_list	conversions;
	int		i;
	int		len;

	if (!str)
		return (-1);
	va_start(conversions, str);
	i = 0;
	len = 0;
	while (str[i])
	{
		if (str[i] == '%')
		{
			i++;
			ft_printcommands(str[i], &conversions, &len, &i);
			i++;
		}
		else
		{
			ft__printf_putchar(str[i], &len);
			i++;
		}
	}
	va_end(conversions);
	return (len);
}
