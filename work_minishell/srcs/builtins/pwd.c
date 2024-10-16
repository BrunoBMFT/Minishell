/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 23:09:17 by bruno             #+#    #+#             */
/*   Updated: 2024/10/16 20:37:10 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	caught_pwd(t_jobs *job)
{
	char	cwd[PATH_MAX];

	if (!getcwd(cwd, PATH_MAX))
		return (printf("getcwd() error\n"), 1);
	ft_putendl_fd(cwd, 1);
	return (0);
}
