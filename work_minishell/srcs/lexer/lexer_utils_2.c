/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:20:43 by ycantin           #+#    #+#             */
/*   Updated: 2024/11/08 05:02:48 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*filename(int i)
{
	char	*num;
	char	*full;

	num = ft_itoa(i);
	full = ft_strjoin(".heredoc_", num);
	free(num);
	return (full);
}

void	assign_values(t_jobs **new, t_token **cur, t_env *env)
{
	if ((*cur)->type > 0 && (*cur)->type < 4)
	{
		(*new)->type = (*cur)->type;
		(*new)->job = NULL;
	}
}
