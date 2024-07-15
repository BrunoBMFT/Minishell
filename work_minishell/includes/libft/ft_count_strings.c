/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_strings.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 12:42:28 by yohan             #+#    #+#             */
/*   Updated: 2024/07/10 13:34:14 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int count_strings(char **array)
{
    int i;

    i = 0;
    if (!array || !*array)
        return (0);
    while (array[i])
        i++;
    return (i);
}
