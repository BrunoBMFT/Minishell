/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 20:54:30 by yohan             #+#    #+#             */
/*   Updated: 2024/06/26 17:45:16 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "printf.h"

void	ft_printcommands(char c, va_list *conversions, int *len, int *i)
{
	if (c == 'c')
		ft_putchar(va_arg(*conversions, int), len);
	else if (c == 's')
		ft_putstr(va_arg(*conversions, char *), len);
	else if (c == '%')
		ft_putchar('%', len);
	else if (c == 'd' || c == 'i')
		ft_putnbr(va_arg(*conversions, int), len);
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
			ft_putchar(str[i], &len);
			i++;
		}
	}
	va_end(conversions);
	return (len);
}
// #include <limits.h>
// int	main(void)
// {
// 	//printf("%d\n", printf("%x", LONG_MAX));
// 	//ft_printf("%d\n\n", ft_printf("%x", LONG_MAX));
// 	char			*memory;
// 	char			*null;
// 	int				minValue;
// 	int				maxValue;
// 	unsigned int	max;
// 	char *i = "cabra";
// 	max = 4294967295;
// 	memory = "hello world";
// 	null = 0;
// 	ft_printf("your charracter: %c\n", 'c');
// 	printf("printf: %c\n\n", 'c');
// 	ft_printf("your string: %s\n", "test 1");
// 	printf("printf: %s\n\n", "test 1");
// 	ft_printf("your number: %d\n", 0);
// 	ft_printf("your number: %d\n", -2147482648);
// 	ft_printf("your number: %d\n", 2147483647);
// 	printf("printf: %d\n", 0);
// 	printf("printf: %d\n", -2147482648);
// 	printf("printf: %d\n\n", 2147482647);
// 	ft_printf("your integer: %i\n", 0);
// 	ft_printf("your integer: %i\n", -2147483648);
// 	ft_printf("your integer: %i\n", 2147482647);
// 	printf("printf: %i\n", 0);
// 	minValue = -2147483648;
// 	printf("printf: %i\n", minValue);
// 	printf("printf: %i\n\n", 2147483647);
// 	maxValue = 2147483647;
// 	ft_printf("your unsigned number: %u\n", max);
// 	ft_printf("your unsigned number: %u\n", 0);
// 	printf("printf: %u\n", max);
// 	printf("printf: %u\n\n", 0);
// 	ft_printf("your HEX number: %X\n", 42);
// 	printf("printf: %X\n\n", 42);
// 	ft_printf("your hex number: %x\n", 42);
// 	printf("printf: %x\n\n", 42);
// 	ft_printf("your number: %p\n", memory);
// 	printf("printf: %p\n\n", memory);
// 	ft_printf("your ptr edge case: %p\n", null);
//     ft_printf("your length: %d\n", ft_printf("%p\n", i));
//     printf("printf: %d\n\n", printf("%p\n", i));
// 	i = "hello worlds";
//     ft_printf("your length: %d\n", ft_printf("%p\n", i));
//     printf("printf: %d\n\n", printf("%p\n", i));
// 	int si = 9;
//     ft_printf("your length: %d\n", ft_printf("%d\n", si));
//     printf("printf: %d\n\n", printf("%d\n", si));
// 	i = "bviuriru";
//     ft_printf("your length: %d\n", ft_printf("%s\n", i));
//     printf("printf: %d\n\n", printf("%s\n", i));
//     ft_printf("your length: %d\n", ft_printf("%X\n", minValue));
//     printf("printf: %d\n\n", printf("%X\n", minValue));
// 	ft_printf("your length: %d\n", ft_printf("%X\n", maxValue));
//     printf("printf: %d\n\n", printf("%X\n", maxValue));
// 	ft_printf("your length: %d\n", ft_printf("%x\n", minValue));
//     printf("printf: %d\n\n", printf("%x\n", minValue));
// 	ft_printf("your length: %d\n", ft_printf("%x\n", maxValue));
//     printf("printf: %d\n\n", printf("%x\n", maxValue));
// 	ft_printf("%d\n", ft_printf("%p%p", NULL, NULL));
// 	printf("%d\n\n", printf("%p%p", NULL, NULL));
// 	printf("%d\n", ft_printf(NULL));
// 	printf("%d", printf(NULL));
// 	ft_printf("%d", ft_printf("%s\n", "hello", "hello"));
	
// 	return (0);
// }

// int main(void)
// {
// 	ft_printf("%x\n%x", -15, 15);
// }