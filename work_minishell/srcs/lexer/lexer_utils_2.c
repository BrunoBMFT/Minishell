/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 15:03:22 by bruno             #+#    #+#             */
/*   Updated: 2024/11/13 15:04:12 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*filename(int i)
{
	char *num;
	char *full;

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
