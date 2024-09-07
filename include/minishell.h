/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen <danjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 07:52:36 by danjimen          #+#    #+#             */
/*   Updated: 2024/09/07 19:25:18 by danjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// INCLUDES

//libft
#include "libft/libft.h"

//max int y min int
#include <stdint.h>
#include <limits.h>

// printf, readline, perror
#include <stdio.h>

// rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay, add_history
#include <readline/readline.h>
#include <readline/history.h>

// malloc, free, exit, getenv
#include <stdlib.h>

// write, access, read, close, fork, getcwd, chdir, unlink, execve, dup, dup2,
// pipe, isatty, ttyname, ttyslot, tcsetattr, tcgetattr
#include <unistd.h>

// open, stat, lstat, fstat
#include <sys/stat.h>
#include <fcntl.h>

// wait, waitpid, wait3, wait4
#include <sys/wait.h>

// signal, sigaction
#include <signal.h>

// opendir, readdir, closedir
#include <sys/types.h>
#include <dirent.h>

// strerror
#include <string.h>

// ioctl
#include <sys/ioctl.h>

// tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
#include <curses.h>
#include <term.h>

// DEFINES
#define MAX_ARGS 100

// ANSI escape sequences for colors
#define RESET "\001\033[0m\002"
#define RED "\001\033[31m\002"
#define GREEN "\001\033[32m\002"
#define YELLOW "\001\033[33m\002"
#define BLUE "\001\033[34m\002"
#define MAGENTA "\001\033[35m\002"
#define CYAN "\001\033[36m\002"
#define WHITE "\001\033[37m\002"
#define BOLD "\001\033[1m\002"

// GLOBAL VARIABLES
extern volatile sig_atomic_t	g_signal_received;

// STRUCTURES
/* typedef enum e_file_type
{
	INFILE,
	HERE_DOC,
	TRUNC,
	APPEND
}	t_file_type; */

typedef enum e_param_type
{
	CMD,
	INFILE,
	HERE_DOC,
	OUTFILE,
	APPEND,
	PIPE,
	PARAMS,
	BUILTING
}	t_param_type;

typedef struct s_env
{
	char			*variable;
	char			*content;
	int				order;
	struct s_env	*next;
}			t_env;

typedef enum s_bool
{
	t_false,
	t_true,
}	t_bool;

typedef struct s_fd
{
	int	inf;
	int	outf;
}	t_fd;

typedef struct s_pipes
{
	int				num_command;
	int				inf_pipe;
	int				outf_pipe;
	struct s_pipes	*next;
}		t_pipes;

typedef struct s_mini
{
	char	*user_prompt;
	t_pipes	*first_pipe;
	t_fd	in_out;
	int		nbr_env_nodes;
	t_env	*env_first_node;
	t_env	*env_iter;
}	t_mini;

typedef struct s_params
{
	char			*content;
	int				argc;
	t_param_type	type;
	t_bool			quotes;
	struct s_params	*next;
}	t_params;

typedef struct s_args
{
	char		*input;
	char		*input_trimed;
	char		*last_history;
	char		*args[MAX_ARGS];
	t_bool		quotes[MAX_ARGS];
	int			argc;
	char		*arg;
	char		*result;
	size_t		result_capacity;
	t_mini		*mini;
	char		*arg_ptr;
	t_bool		in_single_quote;
	t_bool		in_double_quote;
	t_params	*params;
}	t_args;

/*_____           _        _                         
 |  __ \         | |      | |                        
 | |__) | __ ___ | |_ ___ | |_ _   _ _ __   ___  ___ 
 |  ___/ '__/ _ \| __/ _ \| __| | | | '_ \ / _ \/ __|
 | |   | | | (_) | || (_) | |_| |_| | |_) |  __/\__ \
 |_|   |_|  \___/ \__\___/ \__|\__, | .__/ \___||___/
                                __/ | |              
                               |___/|_|            */

//////////////////////////////////////////////////////
//						MAIN.C						//
//////////////////////////////////////////////////////
void		free_at_exit(t_args *args);
//int		main(void);

// ╔═.✵.═════════════════════════════════════════════╗
// 					PARSE FOLDER
// ╚═════════════════════════════════════════════.✵.═╝

//////////////////////////////////////////////////////
//						PARSE.C						//
//////////////////////////////////////////////////////
int			parse(t_args *args, t_mini *mini);

//////////////////////////////////////////////////////
//					FT_TOKENIZE.C					//
//////////////////////////////////////////////////////
int			add_to_args(t_args *args, int *argc, t_mini *mini);

//////////////////////////////////////////////////////
//					EXPANDER.C						//
//////////////////////////////////////////////////////
char		*expander(t_args *args, t_mini *mini);

//////////////////////////////////////////////////////
//				LIST_OF_PARAMS.C					//
//////////////////////////////////////////////////////
void		update_last_command_env_var(t_args *args);
void		del_params(t_args *args);
t_params	*add_argument_to_list(t_args *args, int *argc);

// ╔═.✵.═════════════════════════════════════════════╗
// 					ENV FOLDER
// ╚═════════════════════════════════════════════.✵.═╝

//////////////////////////////////////////////////////
//					FT_LIST_ENV.C					//
//////////////////////////////////////////////////////
char		*ft_find_env(t_mini *mini, char *find);
void		ft_print_env(t_mini *mini);
void		free_env(t_mini *mini);
int			add_back_env(t_env *node, t_mini *mini);
t_env		*env_new(char *env);

//////////////////////////////////////////////////////
//				FT_ENVIRONMENT.C					//
//////////////////////////////////////////////////////
void		ft_unset_env(char *unset, t_mini *mini);
void		ft_export_env(char *new_env, t_mini *mini);
char		*ft_get_env(char *to_expand, t_mini *mini);
int			ft_set_env(char **env, t_mini *mini);

// ╔═.✵.═════════════════════════════════════════════╗
	// 				BUILT-INS FOLDER
// ╚═════════════════════════════════════════════.✵.═╝

//////////////////////////////////////////////////////
//					BUILT-INS.C						//
//////////////////////////////////////////////////////
int			ft_built_ins(t_args *args, t_mini *mini);

//////////////////////////////////////////////////////
//					FT_ECHO.C						//
//////////////////////////////////////////////////////
void		ft_built_echo(t_args *args);

//////////////////////////////////////////////////////
//					FT_EXIT.C						//
//////////////////////////////////////////////////////
void		ft_built_exit(t_args *args, t_mini *mini);

//////////////////////////////////////////////////////
//					FT_EXPORT.C						//
//////////////////////////////////////////////////////
void		ft_built_export(t_args *args, t_mini *mini);

// ╔═.✵.═════════════════════════════════════════════╗
// 					EXECUTOR FOLDER
// ╚═════════════════════════════════════════════.✵.═╝

//////////////////////////////////////////////////////
//					EXECUTOR.C						//
//////////////////////////////////////////////////////
void		executor(t_args *args);

//////////////////////////////////////////////////////
//					REDIRECTOR.C					//
//////////////////////////////////////////////////////
int			redirector(t_args *args, t_mini *mini);

//////////////////////////////////////////////////////
//					CREATE_PIPES.C					//
//////////////////////////////////////////////////////
void		create_pipes(int n_command, t_mini *mini);
void		print_pipes(t_mini *mini);

//////////////////////////////////////////////////////
//				GET_REDIRECTIONS.C					//
//////////////////////////////////////////////////////
void		get_redirections(t_args *args, t_mini *mini);
void		set_here_doc(t_params *iter_params, t_pipes *iter_pipes, t_mini *mini);