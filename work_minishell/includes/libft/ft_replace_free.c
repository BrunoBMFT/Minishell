/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_replace_free.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 17:44:53 by bruno             #+#    #+#             */
/*   Updated: 2024/11/14 04:36:43 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	replace_and_free(char **target, char *replacement)
{
	if (*target)
		free(*target);
	*target = replacement;
}

void	free_and_allocate(char **target, char *replacement, bool to_free)
{
	if (*target)
		free(*target);
	if (!replacement)
		*target = NULL;
	else
		*target = ft_strdup(replacement);
	if (to_free && replacement)
		free(replacement);
}
