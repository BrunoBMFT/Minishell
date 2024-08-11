/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:20:43 by ycantin           #+#    #+#             */
/*   Updated: 2024/08/10 13:14:04 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* #include "../includes/minishell.h"

void	ctrl_c_idle(int sig)
{
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sigquit(int sig)
{
	(void)sig;
	ft_printf("Quit\n");
}

int	set_signal(int sig, void f(int))
{
	struct sigaction	sa;

	sa.sa_handler = f;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	if (sigaction(sig, &sa, NULL) < 0)
		return (-1);

	return (0);
} */
