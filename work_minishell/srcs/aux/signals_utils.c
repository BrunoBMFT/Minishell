/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 23:20:22 by bruno             #+#    #+#             */
/*   Updated: 2024/12/02 23:26:24 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	eof_sig(char *line, t_env *env)
{
	free (line);
	printf("exit\n");
	if (env->env)
		free_array(env->env);
	rl_clear_history();
	exit (env->status);
}
