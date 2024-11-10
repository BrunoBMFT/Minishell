/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 16:38:21 by ycantin           #+#    #+#             */
/*   Updated: 2024/11/09 18:13:19 by bruno            ###   ########.fr       */
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
# include <sys/stat.h>
# include <sys/types.h>
# include <signal.h>
# include <fcntl.h>
# include <limits.h>
# include <dirent.h>
# include <readline/readline.h>
# include <readline/history.h>

static volatile sig_atomic_t	sig = 0;//global var needs to start with g_

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

typedef enum e_signal//change name of variables
{
	ROOT_SIG,
	CHILD_SIG,
	HEREDOC_SIG,
	IGNORE_SIG,
}						t_signal;

typedef struct s_env
{
	int		status;
	char	*prompt;
	char	**env;
	bool	piped;
	int		saved_stdin;
	int		saved_stdout;
	bool	redir_error_flag;
	pid_t	*pids;
}				t_env;

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
	char	a;
	char	b;
	char	c;
	char	d;
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
	char			*delimiters;
	int				append;
	int				heredoc;
	char			*heredoc_file;
	int				mult_input_flag;
	struct s_jobs	*next;
}	t_jobs;


//tokenizer:
void	tokenize(t_token **list, char *str);
char	**token_array(char *str);
char	**modify_array(char **array, t_env *env);
int		count_words(char *str);
int		define_type(char *str);
t_token	*addtok(void *content);
t_token	*get_last_tok(t_token *lst);
void	go_to_next(t_token **lst, t_token *new);

//lexer:
t_jobs	*build(char *command_line, t_env *env);
t_jobs	*addjob(void *content);
t_jobs	*get_last_job(t_jobs *lst);
void	go_to_next_job(t_jobs **lst, t_jobs *new);
void	make_job_list(t_jobs **job_list, t_token **tok_list, t_env *env);
int		count_tokens_in_job(t_token *cur);

//error_correction:
int		count_quotes(char *str, int *i);
int		count_normal_chars(char *str, int *i);
int		count_special_chars(char *str, int *i);
char	*split_complex_args(char *str);

//expansions:
char	*ft_env_var(char *str);
char	*ft_getenv(char *str, char **env);
char	*expand_env_vars(char *input, t_env *env);
char	*no_expansion(char *str, t_var_holder h);
char	*expand(char *str, t_env *env);
char	*expansion(char *str, t_var_holder *h, t_env *env);
char	*unquote_and_direct(char *str, t_env *env);
bool	is_empty_arg(char *str, char end);


//parser:
int		parse(t_token **token);
char	*parse_quotes(char *line);
int		secondquote(char *line);
int		parse_last_token(char **cmd_line, t_token **list, t_token **last);

//executor
void	start_executor(t_jobs *job, t_env *env);
bool	init_executor(t_jobs *job, t_env *env);
void	executor_input(t_jobs *job, t_env *env);
void	executor_output(t_jobs *job, t_env *env);
void	do_child_process(t_jobs *job, t_env *env);
void	do_simple_process(t_jobs *job, t_env *env);
void	execute_job(t_jobs *job, t_env *env);


//builtins:
int		try_builtins(t_jobs *job, t_env *env);
int		caught_echo(t_jobs *job);
void	caught_cd(t_jobs *job, t_env *env);
int		caught_pwd(void);
int		caught_export(t_jobs *job, t_env *env);
int		caught_unset(t_jobs *job, t_env *env);
int		caught_env(t_jobs *job, t_env *env);
int		caught_printenv(t_env *env);//not needed
int		caught_exit(t_jobs *jobs, t_env *env, bool pipe);

//redirections
void	update_input(t_jobs *job);
int		update_output(t_jobs *job, char **env, char **temp_vars);
int		append_to_file(t_jobs *job, char **env, char **temp_vars);
int		handle_heredoc(t_jobs *job, t_env env);
void	print_file(int fd);

//free:
void	clean_exit(t_jobs *jobs, t_env *env, int status);
void	clear_jobs(t_jobs **lst);
void	clear_list(t_token **lst);
void	clean_up_build(t_token **list, char *cmd_line);
void	free_all(t_token **list, char **array, char *message, int len);

//signals:
void	choose_sig(t_signal type);
void	EOF_sig(char *line, t_env *env);

//aux:
char	*update_prompt(void);
int		ft_getpid(void);
char	**dup_env(char **envp);
void	*ft_calloc_pids(t_jobs *job);
t_env	init_env(char **envp);

void	print_jobs(char *line, t_jobs *jobs);//to remove

#endif