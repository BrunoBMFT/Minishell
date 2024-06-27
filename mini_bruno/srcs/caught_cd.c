/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   caught_cd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 18:10:16 by bruno             #+#    #+#             */
/*   Updated: 2024/06/17 19:24:16 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/minishell.h"


void	change_dir(char **envp, char *dir)
{
	int i = 0;
	while (envp[i])
	{
		if (ft_strnstr(envp[i], "PATH", 4));//len = 4?
			ft_printf("%s\n", envp[i]);
		i++;
	}
//	ft_printf("%s\n", dir);
}
