/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_array.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 10:57:16 by ycantin           #+#    #+#             */
/*   Updated: 2024/06/28 10:59:00 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void    free_array(char **array)
{
    int i;

    i = 0;
    if (!array || !array[i])
        return ;
    while (array[i])
    {
        free(array[i]);
        i++;
    }
    free(array);
}