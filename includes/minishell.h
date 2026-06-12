#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <signal.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	char			*target;
	int				quoted;
	int				fd;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			**args;
	t_redir			*redirs;
	struct s_cmd	*next;
}	t_cmd;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_REDIR_HEREDOC,
	TOKEN_ENV_VAR
}	t_token_type;

typedef enum e_quote_type
{
	QUOTE_NONE,
	QUOTE_SINGLE,
	QUOTE_DOUBLE
}	t_quote_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	t_quote_type	quote_type;
	int				joined;
	int				expanded;
	struct s_token	*next;
}	t_token;

typedef struct s_pipe_ctx
{
	int		pipefd[2];
	int		in_fd;
	int		*exit_status;
	char	**envp;
}	t_pipe_ctx;

typedef struct s_hd_ctx
{
	int		write_fd;
	int		quoted;
	char	**envp;
	int		est;
}	t_hd_ctx;

extern volatile sig_atomic_t	g_signal;

/* signals */
void	set_signal_prompt(void);
void	set_signal_exec(void);
void	set_signal_child(void);
void	set_signal_heredoc(void);

/* execution */
int		is_builtin(t_cmd *cmd);
void	exec_builtin(t_cmd *cmd, char ***envp, int *exit_status);
void	exec_builtin_core(t_cmd *cmd, char ***envp, int *exit_status);
void	exec_cmd(t_cmd *cmd, char **envp, int *exit_status);
void	child_exec(t_cmd *cmd, char **envp);
void	execute(t_cmd *cmd, char ***envp, int *exit_status);
void	pipe_exec(t_cmd *cmd, char ***envp, int *exit_status);
void	pipe_child_dispatch(t_cmd *cmd, char **envp, int *exit_status);
void	update_exit_status(int status, int *exit_status);
int		save_stdio(int *saved_in, int *saved_out);
void	restore_stdio(int saved_in, int saved_out);

/* redirections */
int		setup_redirections(t_cmd *cmd, char **envp, int *exit_status);
int		collect_one_heredoc(t_redir *redir, char **envp, int *exit_status);
int		collect_heredocs(t_cmd *cmd, char **envp, int *exit_status);

/* path */
char	*get_path(char *cmd, char **envp);
void	free_split(char **split);

/* builtins */
void	env_bi(t_cmd *cmd, char **envp, int *exit_status);
void	exit_bi(t_cmd *cmd, int *exit_status);
void	unset_bi(t_cmd *cmd, char ***envp, int *exit_status);
void	export_bi(t_cmd *cmd, char ***envp, int *exit_status);
void	pwd_bi(t_cmd *cmd, int *exit_status);
void	cd_bi(t_cmd *cmd, char ***envp, int *exit_status);
void	echo_bi(t_cmd *cmd, int *exit_status);

/* env utils */
int		env_size(char **envp);
void	free_array(char ***arr);
char	*env_get_value(char **envp, const char *name);
int		env_name_match(const char *entry, const char *name);
void	add_env_variable(char ***envp, const char *var);
void	update_env_variable(char ***envp, const char *name, const char *value);
char	**copy_envp(char **envp);

/* builtin helpers */
int		is_numeric(const char *s);
long	ft_atol(const char *str);
void	unset_process_arg(char ***envp, const char *name);
void	export_handle_arg(char ***envp, const char *arg, int *exit_status);
void	export_print_all(char **envp);
int		is_valid_identifier(const char *str);

/* tokenizing */
t_token	*tokenize(char *input);
t_token	*create_token(t_token_type t, char *v, t_quote_type q);
void	add_token_to_list(t_token **lst, t_token_type t,
			char *v, t_quote_type q);
void	free_tokens(t_token *tokens);
void	merge_joined_tokens(t_token **tokens);
char	*extract_word(char *input, int *i, t_quote_type *qt);
char	*extract_quoted(char *input, int *i, char qc);
int		is_whitespace(char c);
void	skip_spaces(char *input, int *i);
int		is_quote(char c);
int		is_operator(char c);
int		handle_quote(char *input, int *i, t_token **tokens);
int		handle_double_operator(char *input, int *i, t_token **tokens);
int		handle_single_operator(char *input, int *i, t_token **tokens);
int		handle_word(char *input, int *i, t_token **tokens);
int		handle_operator(char *input, int *i, t_token **tokens);

/* commands */
t_cmd	*create_command(void);
void	free_command(t_cmd *cmd);
void	free_commands(t_cmd *cmds);
t_cmd	*build_commands(t_token *tokens);
int		count_args(t_token *tokens);
int		add_arg_to_command(t_cmd *cmd, char *arg, int *idx);
int		count_word_pieces(t_token *token);
int		add_split_word(t_cmd *cmd, t_token *token, int *idx);
int		init_args_array(t_cmd *cmd, int n);
int		handle_redir_in(t_cmd *cmd, t_token **cur);
int		handle_redir_out(t_cmd *cmd, t_token **cur);
int		handle_heredoc(t_cmd *cmd, t_token **cur);
int		add_redir(t_cmd *cmd, t_redir_type type, char *target, int quoted);
void	free_redirs(t_redir *redir);

/* syntax */
int		validate_syntax(char *input, t_token *tokens);
int		has_unclosed_quotes(char *input);
int		validate_pipes(t_token *tokens);
int		validate_redirections(t_token *tokens);

/* expansion */
void	expand_tokens(t_token *tokens, char **envp, int exit_status);
char	*expand_value(char *value, char **envp, int in_squote, int est);
char	*get_var_value(const char *name, char **envp, int exit_status);
int		get_var_name_len(const char *str);
char	*extract_var_name(const char *str, int len);
int		should_expand(const char *v);
char	*build_expanded(const char *s, int *i, const char *vv, int vnl);

/* utils */
int		ft_strcmp(const char *s1, const char *s2);

#endif
