/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   heredoc.c										  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: ycantin <ycantin@student.42.fr>			+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/06/26 18:20:43 by ycantin		   #+#	#+#			 */
/*   Updated: 2024/08/29 18:59:07 by ycantin		  ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "../includes/minishell.h"

int	manage_redirection(t_jobs *job)
{
	int	redirected_input;

	redirected_input = open(".heredoc", O_RDONLY);
	if (redirected_input < 0)
		return (-1);
	if (dup2(redirected_input, STDIN_FILENO) < 0)
	{
		close(redirected_input);
		return (-1);
	}
	close(redirected_input);
	return (0);
}

int	handle_heredoc(t_jobs *job)
{
	int		redirected_input;
	char	*line;

	redirected_input = open(".heredoc", O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (redirected_input < 0)
		return (-1);
	signal(SIGINT, handle_signal_heredoc);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		line = readline("heredoc>");
		if (!line || ft_strcmp(line, job->input) == 0)
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, redirected_input);
		free(line);
	}
	close(redirected_input);
	if (manage_redirection(job) < 0)
		return (-1);
	return (0);
}
