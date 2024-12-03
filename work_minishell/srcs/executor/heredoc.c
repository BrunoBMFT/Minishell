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

#include "../../includes/minishell.h"

bool	heredoc_proc(t_jobs *job, t_env env, int exp, int redirect)
{
	char	*line;	
	char	*temp;

	line = readline("heredoc>");
	if (g_sig == 1)
		return (free (line), false);
	if (!line || ft_strcmp(line, job->delimiters) == 0)
		return (free (line), false);
	if (exp)
	{
		temp = expand(line, &env);
		ft_putendl_fd(temp, redirect);
		free(temp);
	}
	else
		ft_putendl_fd(line, redirect);
	free (line);
	return (true);
}

int	handle_heredoc(t_jobs *job, t_env env)
{
	int		redirected_input;
	int		must_expand;

	must_expand = 1;
	redirected_input = open(job->heredoc_file,
			O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (redirected_input < 0)
		return (-1);
	setup_signal(HEREDOC_SIG);
	heredoc_expand_check(&must_expand, &job, env);
	while (1)
	{
		if (!heredoc_proc(job, env, must_expand, redirected_input))
			break ;
	}
	close (redirected_input);
	return (0);
}
