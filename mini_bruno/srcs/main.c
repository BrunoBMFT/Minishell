/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 20:13:16 by bruno             #+#    #+#             */
/*   Updated: 2024/06/28 17:08:40 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	caught_echo(char *input)
{
	int i = 4;
	while (input[i] == ' ' || input[i] == '\t')
		i++;
	while (input[i])
	{
		write(1, &input[i], 1);
		i++;
	}
}
// int for single vs double quotes
// echo 'hello $USER'
// hello $USER
// echo "hello $USER"
// hello bruno

char	*expand_env_vars(char *input, char **envp)
{
	int i;
	char	**vars = ft_split(input, '$');//if input is just USER, it expands as well
	char	*output = NULL;

	while (*vars)
	{
		i = 0;
		while (envp[i])
		{
			if (ft_strnstr(envp[i], *vars, ft_strlen(*vars)))
			{
				*vars = ft_strtrim(envp[i], *vars);
				*vars = ft_strtrim(*vars, "=");
			}
			i++;
		}
		if (!output)
			output = ft_strdup(*vars);
		else
			output = ft_strjoin(output, *vars);
		vars++;
	}
	if (input[ft_strlen(input) - 1] == '$')
		ft_strcat(output, "$");
//	printf("%s\n", input);//loop the strchr until no return ($USER$USER)
	return (output);
}


void	parse_input(char *input, char **envp)//not parsing, it's more like expander
{
//	ft_printf("%s\n", input);
	
	if (ft_strnstr(input, "echo", 4))
		caught_echo(input);

	input = expand_env_vars(input, envp);
	if (!input)
		return ;
	ft_printf("expanded:\n%s", input);//expander test
}

int	main(int ac, char **av, char **envp)
{
	char	*input;
	t_shell	shell;

	shell.env = envp;

	while (1)
	{
		input = readline("minishell -> ");
		if (input)
			parse_input(input, envp);
		if (ft_strnstr(input, "exit", 4))
			return (rl_clear_history(), free(input), exit(0), 0);
		ft_nl_fd(1);
	}
}