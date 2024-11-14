/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 03:18:24 by bruno             #+#    #+#             */
/*   Updated: 2024/11/14 17:40:05 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ignore_signal(int SIG, struct sigaction *sa)
{
	struct sigaction	original_sa;
	int					original_flags;

	original_flags = sa->sa_flags;
	sa->sa_handler = SIG_IGN;
	sa->sa_flags |= SA_SIGINFO;
	sigemptyset(&sa->sa_mask);
	sigaction(SIG, sa, &original_sa);
	sa->sa_flags = original_flags;
}

void	root_handler(int signal, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	if (signal == SIGINT)
	{
		ft_printf("\n");//1
		rl_on_new_line();//2
		rl_replace_line("", 0);//3
		rl_redisplay();//4
	}
}

void	here_handler(int signal, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	if (signal == SIGINT)
	{
		ft_printf("\n");
		exit (130);
	}
}

void	signal_aux(t_signal type, struct sigaction sa)
{
	if (type == HEREDOC_SIG)
	{
		sa.sa_sigaction = here_handler;
		sa.sa_flags = SA_SIGINFO;
		sigemptyset(&sa.sa_mask);
		sigaction(SIGINT, &sa, NULL);
		ignore_signal(SIGQUIT, &sa);
	}
	else if (type == IGNORE_SIG)
	{
		ignore_signal(SIGINT, &sa);
		ignore_signal(SIGQUIT, &sa);
	}
}
void	setup_signal(t_signal type)
{
	static struct sigaction	sa;

	if (type == ROOT_SIG)
	{
		sa.sa_sigaction = root_handler;
		sa.sa_flags = SA_SIGINFO;//additional info about the signal will be passed to handler
	//inits the set, no additional signals are blocked while sigint is being handled
		sigemptyset(&sa.sa_mask);
		sigaction(SIGINT, &sa, NULL);
		ignore_signal(SIGQUIT, &sa);
	}
//basically this child type is for signals to work how they usually work in shell, 
//	thats why we default the handlers for sa. 
//when sigaction is called, it will associate the sa (with handler) to the action (SIGINT etc)
	else if (type == CHILD_SIG)
	{
// SIGFDFL puts every single back to default, instead of the changes we are making here
		sa.sa_handler = SIG_DFL;
		sa.sa_flags = 0;
		sigemptyset(&sa.sa_mask);
		sigaction(SIGINT, &sa, NULL);
		sigaction(SIGQUIT, &sa, NULL);
	}
	else
		signal_aux(type, sa);
}

void	EOF_sig(char *line, t_env *env)
{
	free (line);
	printf("exit\n");
	if (env->env)
		free_array(env->env);
	rl_clear_history();
	exit (env->status);
}
