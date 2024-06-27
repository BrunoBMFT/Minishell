/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 20:13:16 by bruno             #+#    #+#             */
/*   Updated: 2024/06/27 01:05:11 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
//fds for every print function
/*
void	caught_echo(char **vars, char **envp)
{
	int	i = 1;
	if (!vars[1])
		return ;
	while (vars[i])
	{
		ft_putstr_fd(vars[i], 1);
		if (vars[i + 1])//check for last string
			write(1, " ", 1);
		i++;
	}
	
}

char	*env_var_value(char *str, char **envp, int start)
{
	printf("sd\n");
	int i = start;
	while (str[i])
	{
		int j = 0;
		while (envp[j])
		{
			if (ft_strnstr(envp[i], str, ft_strlen(str)) && envp[i][ft_strlen(str)] == '=')
			{
				return (ft_substr(str, i + 1, ft_strlen(str)));
			}
			j++;
		}
		i++;
	}
	return "";
}

//!
char *get_env_value(char *key, char **envp) {
    int i = 0;
    int len = ft_strlen(key);
    while (envp[i]) {
        if (strncmp(envp[i], key, len) == 0 && envp[i][len] == '=')//envp[i] == key && envp[i] == {KEY}=
            return (envp[i] + len + 1); // Return pointer to envp[i]
        i++;
    }
    return "";
}
//!

char *expand_env_vars(char *input, char **envp)
{
	char	*new = NULL;

	int	i = 0, j = 0;//use pointers
	char	**vars = ft_split(input, ' ');
	while (vars[i])
	{
		j = 0;
		while (vars[i][j])
		{
			if (vars[i][j] == '$')
				vars[i] = env_var_value(vars[i], envp, j);
			j++;
		}
		if (!new)
			new = ft_strdup(vars[i]);
		else
		{
			new = ft_strjoin(new, " ");
			new = ft_strjoin(new, vars[i]);
		}
		i++;
	}
//	ft_split_free(vars);
	return (new);
}
*/
//check for unclosed speech marks
void	caught_echo(char *input)
{
	int i = 0;
	input = ft_strtrim(input, "echo ");
	char	**vars = ft_split(input, ' ');
	if (ft_split_wordcount(vars) == 1)//fix echo run vs echo "hello world"
	{
		parse_quotation_mark(vars[0]);
		ft_printf("%s", vars[0]);
	}
	/*else
	{
		while (vars[i])
		{
			ft_printf("%s", vars[i]);
			i++;
		}
	}*/
	
	ft_split_free(vars);
}


void	parse_input(char *input, char **envp)//not parsing, it's more like expander
{
	ft_printf("%s\n", input);
//	if (!(parse_quotation_mark(input)))
//		return ;//error
	if (ft_strnstr(input, "echo", 4))
		caught_echo(input);
	
//	input = expand_env_vars(input, envp);
//	if (!input)
//		return ;
//	ft_printf("expanded:\n %s", input);//expander test
}

int	main(int ac, char **av, char **envp)
{
	char	*input;

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