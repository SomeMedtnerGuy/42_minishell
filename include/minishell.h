/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 13:30:43 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/07/30 12:43:59 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// INCLUDES
# include <readline/readline.h> // readline
# include <errno.h> // error codes
# include <signal.h> // signals
# include <sys/wait.h> // wait for child process
# include <fcntl.h> // file manipulation
# include "../libft/libft.h" // libft

// MESSAGES
# define LAUNCH_ERROR "minishell does not accept extra options. \
Please launch it without arguments.\n"
# define PROMPT "\nEnter your command: "
# define CTRD_EXIT_MSG "exit\n"
# define SYNTAX_ERROR "syntax error near unexpected token `"
# define SYNTAX_ERROR_END "syntax error near unexpected token `newline'\n"

// TYPES OF NODES
# define REDIR 0
# define EXEC 1
# define PIPE 2

// MISCELLANEOUS DEFINES
# define SPACES " \t\n\v\f\r"
# define TOKEN_CHARS "<>|"
# define REDIR_TYPES "<>+-"
# define SET 0
# define GET 1
# define BUILTINS_AM 7
# define ENVP_FILENAME ".tempfiles/env_filename_because_we_gotta_find_the\
_dumbest_ways_to_get_around_this_damn_subject"

typedef int	(*t_builtin)(char **, char ***);

// STRUCTS
// Syntax handler
typedef struct s_flags
{
	int	sq;
	int	dq;
	char	prev;
	int	error;
}	t_flags;

// Tokenizer
typedef struct s_token
{
	char			type;
	char			*content;
	struct s_token	*next;
}	t_token;

typedef struct s_tokenizer_data
{
	t_token	*token_lst;
	char	*ptr;
	char	type;
	char	*stoken;
	int		status;
}	t_tokenizer_data;

// Tree builder
typedef struct s_node
{
	int	type;
}	t_node;

typedef struct s_redir
{
	int				type;
	char			*file;
	char			redir_type;
	struct s_redir	*next;
}	t_redir;

typedef struct s_exec
{
	int		type;
	t_list	*argv;
	t_redir	*redirs;
	char	**envp; //TODO; necessary?
}	t_exec;

typedef struct s_pipe
{
	int		type;
	t_node	*left;
	t_node	*right;
}	t_pipe;

// Root struct
typedef struct s_root
{
	char	*line;
	char	**envp;
	t_token	*token_lst;
	t_node	*tree;
	char	*cwd;
	int		exit_code;
	int		prev_exit_code;
}	t_root;

// FREE_EXIT.C
void		free_exit(t_root *r, int exit_code);

// SIGNALS.C
int			setget_signo(int action, int ntoset);
void		set_sig_hd(void);
void		signal_handler(int signo);
void		psignal_handler_hd(int signo);
void		set_signals(void);

// HANDLE_SYNTAX.C
int			handle_syntax(char *ptr);

// TOKENIZE_LINE.C
void		tokenize_line(t_root *r);
void		handle_syntax_error(t_tokenizer_data *td,
				int *exit_code, char *line);

// TOKENIZE_LINE2.C
int			update_token(char **token, char *start);
void		expand_cmd_env(t_tokenizer_data *td, t_root *r);

// TOKENIZER_PARSERS.C
void		parse_quotes(t_tokenizer_data *td, t_root *r, char c);
void		parse_redirs_pipes(t_tokenizer_data *td, t_root *r);
void		parse_spaces(t_tokenizer_data *td, t_root *r);

// TOKENIZER_EXIT_FREE.C
void		free_tokenizer(t_tokenizer_data *td);
void		exit_from_tokenizer(t_tokenizer_data *td, t_root *r,
				char *msg, int exit_code);

// GET_ENV_VALUE.C
int			get_env_key_len(char *start);
char		*get_env_key(char *start);
char		*get_env_value(char *start, char **envp);

// TOKENLST_HELPERS.C
t_token		*tokennew(char type, char *content);
t_token		*tokenlast(t_token *token);
void		tokenadd_back(t_token **token, t_token *new);
int			token_createadd(t_token **tokenlst, char type, char *tokenstr);
void		free_tokenlst(t_token *tokenlst);

// BUILD_TREE.C
int			build_tree(t_root *r);

// NODE_CONSTRUCTORS.C
t_redir		*create_redir(char *file, char redir_type);
t_exec		*create_exec(char **envp);
t_pipe		*create_pipe(t_node *left, t_node *right);

// SET_HEREDOCS.C
int			set_heredocs(t_node *node, t_root *r);

// HEREDOC.C
char		*heredoc(char *eof, t_root *r);

// HEREDOC_HELPERS.C
int			get_next_rn(void);
char		*find_and_expand(char *line);

// CREATE_HEREDOC_FILE.C
void		create_heredoc_file(char *filename, char *eof);

// GET_ENV_VALUE_HD.C
char    *get_env_value_hd(char *start);

// FREE_TREE.C
void		free_tree(t_node *node);

// EXECUTE_NODE.C
char	**create_args(t_list *argv);
void		execute_node(t_node *node, t_root *r);

// COMMAND_HELPERS.C
char		*validate_cmd(char *cmd, char **env);

// BUILTIN_UTILS.C
int			run_builtin(t_list *argv_lst, char ***envp);
t_builtin	get_builtin(char *cmd);
void		delete_var(char *var, char **envp);
int			get_envp_i(char *key, char **envp);
int			is_key_valid(char *key);
// BUILTINS
int			ft_echo(char **argv, char ***envp);
int			ft_cd(char **argv, char ***envp);
int			ft_pwd(char **argv, char ***envp);
int			ft_export(char **argv, char ***envp);
int			ft_unset(char **argv, char ***envp);
int			ft_env(char **argv, char ***envp);
int			ft_exit(char **argv, char ***envp);

#endif
