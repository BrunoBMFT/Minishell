/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 17:32:30 by bruno             #+#    #+#             */
/*   Updated: 2024/07/16 22:09:00 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* #include "../includes/minishell.h"
#include <signal.h>

void    handle_sigint(int sig)
{
	if (sig == 1)//sigint expands to 2
		kill(getpid(), sig);
}

void    ctrl_c_idle(int sig)
{
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

int set_up_signal(void f(int))
{
	struct sigaction sa;

	sa.sa_handler = f;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGINT, &sa, NULL) < 0)
		return (-1);

	return (0);
} */