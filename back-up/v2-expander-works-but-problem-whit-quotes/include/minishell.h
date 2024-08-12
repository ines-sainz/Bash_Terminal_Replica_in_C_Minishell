/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen <danjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 07:52:36 by danjimen          #+#    #+#             */
/*   Updated: 2024/08/12 19:43:52 by danjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// DEFINES

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

// Secuencias de escape ANSI para colores
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define BOLD "\033[1m"

// DEFINES
#define MAX_ARGS 100

// STRUCTURES
typedef enum e_param_type
{
	CMD,
	IN_OUT,
	PIPE,
	PARAMS
}	t_param_type;

typedef enum e_file_type
{
	INFILE,
	HERE_DOC,
	TRUNC,
	APPEND
}	t_file_type;

typedef struct s_env
{
	char			*variable;
	char			*content;
	struct s_env	*next;
}			t_env;

typedef enum s_bool
{
	t_false,
	t_true,
}	t_bool;

typedef struct s_fd
{
	int		inf;
	int		outf;
}	t_fd;

typedef struct s_pipes
{
	int	num_pipes;
	int	inf_pipe;
	int	outf_pipe;
}		t_pipes;

typedef struct s_mini
{
	char	*user_prompt;
	t_pipes	pipes;
	t_fd	in_out;
	t_env	*env_first_node;
	t_env	*env_iter;
}	t_mini;

typedef struct s_params
{
	t_param_type	param_type;
	int				argc;
	char			*content;
}	t_params;

typedef struct s_args
{
	char		*input;
	char		*input_trimed;
	char		*args[MAX_ARGS];
	int			argc;
	char		*arg;
	char		*arg_ptr;
	int			in_single_quote;
	int			in_double_quote;
	t_params	*argv;
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
//int		main(void);

//////////////////////////////////////////////////////
//						PARSE.C						//
//////////////////////////////////////////////////////
int		parse(t_args *args, t_mini *mini);

//////////////////////////////////////////////////////
//					FT_TOKENIZE.C					//
//////////////////////////////////////////////////////
void	ft_tokenize(t_args *args, int *argc);
//void	ft_tokenize(char *input, char *args[], int *argc);
//char	**ft_tokenize(char const *s, char c);

//////////////////////////////////////////////////////
//				FT_ENVIRONMENT.C					//
//////////////////////////////////////////////////////
void	ft_unset_env(char *unset, t_mini *mini);
void	ft_export_env(char *new_env, t_mini *mini);
char	*ft_get_env(char *to_expand, t_mini *mini);
t_env	*env_new(char *env);
int		add_back_env(t_env *node, t_mini *mini);
void	free_env(t_mini *mini);
void	ft_print_env(t_mini *mini);
int		ft_set_env(char **env, t_mini *mini);

//////////////////////////////////////////////////////
//					EXPANDER.C						//
//////////////////////////////////////////////////////
//char	*expander(char *input);
char	*expander(char *input, bool in_single_quote);

//////////////////////////////////////////////////////
//					BUILT-INS.C						//
//////////////////////////////////////////////////////
char	*buit_ins(char *command, char *argv, t_mini *mini);