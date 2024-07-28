/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 16:38:21 by ycantin           #+#    #+#             */
/*   Updated: 2024/07/27 20:59:03 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL
# define MINISHELL

# include "libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <unistd.h>
# include <signal.h>
# include <sys/wait.h>
# include <signal.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>

static volatile sig_atomic_t sig = 0;

# define WRITE 1
# define READ 0

# define BEFORE 0
# define AFTER 1

typedef enum types
{
    WORD,
    PIPE,
    AND,
    OR,
    INPUT,
    OUTPUT,
    HEREDOC,
    APPEND_OUT
}               types;

typedef struct s_holder
{
    int i;
    int j;
    int wc;
    int k;
    char **array;
}t_holder;

typedef struct s_token
{
    char                *token;
    int                 pos;
    int                 type;
    struct  s_token     *prev;
    struct  s_token     *next;
}t_token;

typedef struct s_jobs
{//have fdin and fdout and redirs always here?
	int				type;
	char			*cmd;
	char			*execd;
	char			**job;
	struct s_jobs	*next;
}t_jobs;

//bruno:
int	caught_echo(t_jobs *job);
int	caught_cd(t_jobs *job, char **env);
int	caught_pwd(t_jobs *job, char **env);
int	caught_env(t_jobs *job, char **env);
int	caught_unset(t_jobs *job, char **env, char **temp_vars);
int	caught_export(t_jobs *job, char **env, char **temp_vars);

char	*update_prompt();
void	cd_update_pwd(char **env, bool when);

//char	**add_to_env(char **strs, char **temp_vars, char **env, bool is_export);
char	**add_to_env(char **strs, char **temp_vars);
char	*expand_env_vars(char *input, char **env, char **temp_vars);
//char	**variable_declaration(char **str, char **vars);

//executor
int	    start_executor(t_jobs *job, char **env, char **temp_vars);
//void	run_execution(t_jobs *curr, char **env);
int     try_builtins(t_jobs *job, char **env, char **temp_vars);
int	    child_process(t_jobs *job, char **env);
int	    simple_process(t_jobs *job, char **env);
char	*find_path(char *command, char **env);
int	    new_fork(void);
void	panic(char *s);

//to remove
int	clear_proc(char **env);

//free:
void	clear_list(t_token **lst);
void	clean_exit(t_jobs *jobs, char *line, char *prompt);
void	free_all(t_token **list, char **array, char *message, int len);
void print_jobs(t_jobs *jobs);
void    parse(t_token **token);
int         count(char *str);
char        *split_complex_args(char *str);
char        **token_array(char *str);
void        tokenize(t_token **list, char *str);
int	        define_type(char *str);
//lexer:
t_token	    *addtok(void *content);
t_token	    *get_last_tok(t_token *lst);
void	    go_to_next(t_token **lst, t_token *new);
int         peek_next(t_token *list);
char        *full_cmd(t_token **cur);
int         give_type(t_token *token);
t_jobs	    *addjob(void *content);
t_jobs	    *get_last_job(t_jobs *lst);
void	    go_to_next_job(t_jobs **lst, t_jobs *new); 
void        make_job_list(t_jobs **job_list, t_token **tok_list);
void	    clear_jobs(t_jobs **lst);
t_jobs      *build(char *command_line);
void        ctrl_c_idle(int sig);
int         set_signal(int sig, void f(int));
void	check_exit(char *line);
void        start_heredoc(t_jobs *curr);


#endif