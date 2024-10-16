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

void	heredoc_expand_check(int *expand_flag, t_jobs **job)
{
	char *temp;

	if ((*job)->delimiters[0] == '\'' || (*job)->delimiters[0] == '\"')
	{
		temp = ft_substr((*job)->delimiters, 1, (ft_strlen((*job)->delimiters) - 2));
		free((*job)->delimiters);
		(*job)->delimiters = temp;
		*expand_flag = 0;
	}
}

int	handle_heredoc(t_jobs *job, t_env env)
{
	int		redirected_input;
	char	*line;
	char	**delimiters;
	int		i;
	int		must_expand;
	int		max;

	i = 0;
	must_expand = 1;
	max = 0;
	redirected_input = open(job->heredoc_file, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (redirected_input < 0)
		return (-1);
//	signal(SIGINT, handle_signal_heredoc);
//	signal(SIGQUIT, SIG_IGN);
	choose_signal(HEREDOC_SIG);
	choose_signal(IGNORE_SIG);
	heredoc_expand_check(&must_expand, &job);
	printf("%d\n", must_expand);
	while (1)
	{
		line = readline("heredoc>");
		if (!line || ft_strcmp(line, job->delimiters) == 0)
		{
			free(line);
			break ;
		}
		if (must_expand)
		{
			char *temp = expand(line, &env);
			ft_putendl_fd(temp, redirected_input);
			free(temp);

		}
		else	
			ft_putendl_fd(line, redirected_input);
		free(line);
	}
	close(redirected_input);
	return (0);
}


// int	handle_heredoc(t_jobs *job)
// {
// 	int		redirected_input;
// 	char	*line;
// 	char	**delimiters;
// 	int		i;
// 	int		max;
// 	i = 0;
// 	max = 0;
// 	delimiters = ft_split(job->delimiters, ' ');
// 	while (delimiters[max])
// 		max++;
// 	redirected_input = open(".heredoc", O_CREAT | O_RDWR | O_TRUNC, 0644);
// 	if (redirected_input < 0)
// 		return (-1);
// 	signal(SIGINT, handle_signal_heredoc);
// 	signal(SIGQUIT, SIG_IGN);
// 	while (i < (max - 1))
// 	{
// 		while (1)
// 		{
// 			line = readline("fake heredoc>");
// 			if (!line || ft_strcmp(line, delimiters[i]) == 0)
// 			{
// 				free(line);
// 				break ;
// 			}
// 			free(line);
// 		}
// 		i++;
// 	}
// 	while (1)
// 	{
// 		line = readline("heredoc>");
// 		if (!line || ft_strcmp(line, delimiters[i]) == 0)
// 		{
// 			free(line);
// 			break ;
// 		}
// 		ft_putendl_fd(line, redirected_input);
// 		free(line);
// 	}
// 	close(redirected_input);
// 	free_array(delimiters);
// 	/* if (manage_redirection(job) < 0)
// 		return (-1); */
// 	return (0);
// }