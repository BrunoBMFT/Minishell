/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 03:18:24 by bruno             #+#    #+#             */
/*   Updated: 2024/10/12 14:15:05 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//goncalo
//nao funciona

void	ignore_signal(struct sigaction *sa, int signal)
{
	struct sigaction	original_sa;
	int					original_flags;

	original_flags = sa->sa_flags;
	sa->sa_handler = SIG_IGN;
	sa->sa_flags |= SA_SIGINFO;
	if (sigemptyset(&sa->sa_mask) != 0)
		return ;
	sigaction(signal, sa, &original_sa);
	sa->sa_flags = original_flags;
}

void	root_handler(int signal, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	if (signal == SIGINT)
	{
		ft_printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
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

void	choose_signal_aux(t_signal type, struct sigaction sa)
{
	if (type == HEREDOC_SIG)
	{
		sa.sa_sigaction = here_handler;
		sa.sa_flags = SA_SIGINFO;
		if (sigemptyset(&sa.sa_mask) != 0)
			return ;
		sigaction(SIGINT, &sa, NULL);
		ignore_signal(&sa, SIGQUIT);
	}
	else if (type == IGNORE_SIG)
	{
		ignore_signal(&sa, SIGINT);
		ignore_signal(&sa, SIGQUIT);
	}
}
void	choose_signal(t_signal type)
{
	static struct sigaction	sa;

	if (type == ROOT_SIG)
	{
		sa.sa_sigaction = root_handler;
		sa.sa_flags = SA_SIGINFO;
		if (sigemptyset(&sa.sa_mask) != 0)
			return ;
		sigaction(SIGINT, &sa, NULL);
		ignore_signal(&sa, SIGQUIT);
	}
	else if (type == CHILD_SIG)
	{
		sa.sa_handler = SIG_DFL;
		sa.sa_flags = 0;
		if (sigemptyset(&sa.sa_mask) != 0)
			return ;
		sigaction(SIGINT, &sa, NULL);
		sigaction(SIGQUIT, &sa, NULL);
	}
	else
		choose_signal_aux(type, sa);
}

//mine

//can call clean_exit and maybe have exit print a line for why its closing
void	ctrld(char *line, t_env *env)//receives line to free it
{
	free (line);
	printf("exit\n");
	free_array(env->env);//if
	rl_clear_history();
	exit (env->status);
}

//old
/* 
void	sigquit(int sig)
{
	(void)sig;
	write(1, "Quit:\n", 7);
}

void	setup_sigquit_handler(void)//not called
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	sigaction(SIGQUIT, &sa, NULL);
}

// Example signal handlers
void	handle_signal_main(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	handle_signal_heredoc(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	close(STDIN_FILENO);
}

void	handle_signal_child(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
} */
