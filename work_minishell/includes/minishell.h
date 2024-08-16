/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 16:38:21 by ycantin           #+#    #+#             */
/*   Updated: 2024/08/16 01:17:49 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <unistd.h>
# include <signal.h>
# include <sys/wait.h>
# include <signal.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/readline.h>
# include <readline/history.h>

static volatile sig_atomic_t	sig = 0;

# define WRITE 1
# define READ 0

# define BEFORE 0
# define AFTER 1
# define PATH_MAX 4096

typedef enum s_types
{
	WORD,
	PIPE,
	AND,
	OR,
	INPUT,
	OUTPUT,
	HEREDOC,
	APPEND_OUT,
	EXPORT
}			t_types;

typedef struct variable_holder
{
	char	*new;
	char	*before;
	char	*quoted;
	char	*after;
	char	*temp;
	char	*temp2;
	char	*expanded;
	char	*result;
	int		i;
	int		start;
	int		j;
	int		wc;
	int		k;
	char	**array;
}				t_var_holder;

typedef struct s_token
{
	char			*token;
	int				type;
	struct s_token	*next;
}				t_token;

typedef struct s_jobs
{
	int				type;
	char			**job;
	char			*input;
	char			*output;
	int				append;
	int				heredoc;
	int				mult_input_flag;
	struct s_jobs	*next;
}				t_jobs;

//builtins:
int		try_builtins(t_jobs *job, char **env, char ***temp_vars);
int		caught_echo(t_jobs *job);
int		caught_cd(t_jobs *job, char **env);
int		caught_pwd(t_jobs *job, char **env);
int		caught_env(t_jobs *job, char **env);
int		caught_unset(t_jobs *job, char **env, char **temp_vars);
int		caught_export(t_jobs *job, char **env, char **temp_vars);

//prompt:
char	*update_prompt(void);

//expansions:
char	*ft_env_var(char *str);
char	*ft_getenv(char *str, char **env);
char	*expand_env_vars(char *input, char **env, char **temp_vars);
char	*no_expansion(char *str, t_var_holder h);
char	*expansion(char *str, t_var_holder *h, char **env, char **temp_vars, int status);
char *unquote_and_direct(char *str, char **env, char **temp_vars, int status);

//executor
int		start_executor(t_jobs *job, char **env, char ***temp_vars);
int		child_process(t_jobs *job, char **env, char ***temp_vars);
int		simple_process(t_jobs *job, char **env, char ***temp_vars);
int	execute_job(char **command, char **env);
int		new_fork(void);
void	panic(char *s);

//redirections
void	update_input(t_jobs *job);
int		update_output(t_jobs *job, char **env, char ***temp_vars);
int		append_to_file(t_jobs *job, char **env, char ***temp_vars);
void	handle_heredoc(char *delimiter);
void    print_file(int fd);

//free:
void	clear_list(t_token **lst);
void	clean_exit(t_jobs *jobs, char *line, char *prompt);
void	free_all(t_token **list, char **array, char *message, int len);
void	clear_jobs(t_jobs **lst);
void	clean_up_build(t_token **list, char *cmd_line);

//aux:
void	print_jobs(t_jobs *jobs);

//parser:
int		parse(t_token **token);
char	*parse_quotes(char *line);
int		parse_last_token(char **cmd_line, t_token **list, t_token **last);

//tokenizer:
int		count(char *str);
char	*split_complex_args(char *str);
int		assign_i(char *str, int i);
int		count_words(char *str);
char	**token_array(char *str);
void	tokenize(t_token **list, char *str);
int		define_type(char *str);
t_token	*addtok(void *content);
t_token	*get_last_tok(t_token *lst);
void	go_to_next(t_token **lst, t_token *new);

//job list:
t_jobs	*addjob(void *content);
t_jobs	*get_last_job(t_jobs *lst);
void	go_to_next_job(t_jobs **lst, t_jobs *new);
void	make_job_list(t_jobs **job_list, t_token **tok_list, char **env);
t_jobs	*build(char *command_line, char **env, int status);

//signals:
void	ctrl_c_idle(int sig);
int		set_signal(int sig, void f(int));
void	sigquit(int sig);

void	check_exit(char *line);

#endif