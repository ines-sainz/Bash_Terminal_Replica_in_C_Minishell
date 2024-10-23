/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen & isainz-r <danjimen & isainz-    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 15:25:44 by danjimen          #+#    #+#             */
/*   Updated: 2024/10/23 13:14:37 by danjimen &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

volatile sig_atomic_t	g_signal_received = 0;

static void	free_args_at_exit(t_args *args)
{
	if (args->input)
	{
		free (args->input);
		args->input = NULL;
	}
	if (args->arg)
	{
		free (args->arg);
		args->arg = NULL;
	}
	if (args->mini->user_prompt)
	{
		free (args->mini->user_prompt);
		args->mini->user_prompt = NULL;
	}
	if (args->mini->exe_command)
	{
		exe_struct_free (args->mini);
		args->mini->exe_command = NULL;
	}
	if (args->last_history)
	{
		free(args->last_history);
		args->last_history = NULL;
	}
}

void	free_at_exit(t_args *args)
{
	close(args->mini->standard_fds[0]);
	close(args->mini->standard_fds[1]);
	free_args_at_exit(args);
	free_env(args->mini);
	del_params(args);
	rl_clear_history();
}

static void	error_mini_use(int argc, char **argv)
{
	int	i;

	if (argc != 1)
	{
		printf("%s: ", argv[0]);
		i = 1;
		while (i < argc)
		{
			printf("%s", argv[i++]);
			if (i != (argc))
				printf(", ");
		}
		ft_dprintf(2, "minishell: Not flags or args are allowed\n");
		exit (1);
	}
}

void	signal_sigint(int sig)
{
	g_signal_received = sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_eof(void)
{
	g_signal_received = -1;
}

void	initialize_structs(t_args *args, t_mini *mini, char **env)
{
	ft_bzero(args, sizeof(t_args));
	ft_bzero(mini, sizeof(t_mini));
	ft_set_env(env, mini);
	args->mini = mini;
	mini->standard_fds[0] = dup(STDIN_FILENO);
	mini->standard_fds[1] = dup(STDOUT_FILENO);
}

void	create_minim_env_vars(t_mini *mini)
{
	char	*cwd;
	char	*join;

	ft_export_env("?=0", mini);
	cwd = getcwd(NULL, 0);
	join = ft_strjoin("PWD=", cwd);
	ft_export_env(join, mini);
	free (cwd);
	free (join);
	if (ft_find_env(mini, "PATH") == NULL)
		ft_export_env(PATH, mini);
}

int	create_prompt(t_mini *mini)
{
	char	*entry;

	if (getenv("USER") == NULL)
		entry = ft_strdup("user@minishell> ");
	else
		entry = ft_strjoin(getenv("USER"), "@minishell> ");
	mini->user_prompt = malloc(ft_strlen(RED) + ft_strlen(BOLD)
			+ ft_strlen(entry) + ft_strlen(RESET) + 1);
	if (!mini->user_prompt)
	{
		ft_dprintf(2, "Error allocating memory\n");
		free_env(mini);
		return (ERR);
	}
	ft_strcpy(mini->user_prompt, RED);
	ft_strlcat(mini->user_prompt, BOLD, ft_strlen(RED) + ft_strlen(BOLD)
		+ ft_strlen(entry) + ft_strlen(RESET) + 1);
	ft_strlcat(mini->user_prompt, entry, ft_strlen(RED) + ft_strlen(BOLD)
		+ ft_strlen(entry) + ft_strlen(RESET) + 1);
	ft_strlcat(mini->user_prompt, RESET, ft_strlen(RED) + ft_strlen(BOLD)
		+ ft_strlen(entry) + ft_strlen(RESET) + 1);
	free (entry);
	return (OK);
}

int	initialize_main_loop(t_mini *mini, t_args *args, int is_piped)
{
	dup2(mini->standard_fds[0], STDIN_FILENO);
	dup2(mini->standard_fds[1], STDOUT_FILENO);
	g_signal_received = 0;
	if (!is_piped)
		args->input = readline(mini->user_prompt);
	else
		args->input = get_next_line(STDIN_FILENO, t_false);
	if (g_signal_received == SIGINT)
	{
		ft_export_env("?=130", mini);
		g_signal_received = 0;
	}
	if (!args->input)
	{
		handle_eof();
		return (ERR);
	}
	return (OK);
}

void	histcontrol(t_args *args)
{
	if ((args->input[0] != '\0'
			&& ft_strcmp(args->input, args->last_history) != 0)
		|| args->last_history == NULL)
	{
		if (args->last_history != NULL)
			free (args->last_history);
		args->last_history = ft_strdup(args->input);
		add_history(args->input);
	}
}

void	trim_input(t_args *args)
{
	args->input_trimed = ft_strtrim(args->input, " \t\n\r\f\v");
	free(args->input);
	args->input = args->input_trimed;
	args->input_trimed = NULL;
}

void	free_args_in_syntax_error(t_args *args)
{
	int	i;

	i = 0;
	if (args->args[i])
	{
		while (args->args[i])
		{
			printf("DB: Libero %s por syntax error\n", args->args[i]);
			free(args->args[i]);
			args->args[i] = NULL;
			i++;
		}
	}
}

int	closing_minishell(int is_piped, t_args *args)
{
	static int	status;

	ft_dprintf(2, "DB: args->input => %s\n", args->input);
	if (!is_piped)
		printf("exit\n");
	else if (is_piped && access(args->input, F_OK) != 0)
	{
		ft_dprintf(2, "DB: El archivo no existe!\n");
		free_at_exit(args);
		status = 127;
	}
	else
		status = 0;
	/* if (status == 127 && args->input == NULL)
		status = 127; */
	free_at_exit(args);
	return (status);
}

// cc signals.c -lreadline
int	main(int argc, char **argv, char **env)
{
	t_args	args;
	t_mini	mini;
	int		is_piped;

	initialize_structs(&args, &mini, env);
	error_mini_use(argc, argv);
	is_piped = !isatty(STDIN_FILENO);
	if (create_prompt(&mini) == ERR)
		return (ERR);
	signal(SIGINT, signal_sigint);
	signal(SIGQUIT, SIG_IGN);
	create_minim_env_vars(&mini);
	while (1)
	{
		if (initialize_main_loop(&mini, &args, is_piped) == ERR)
			break ;
		histcontrol(&args);
		trim_input(&args);
		if (args.input != NULL && args.input[0] != '\0')
			parse(&args, &mini);
		signal(SIGINT, signal_sigint);
		free(args.input);
		del_params(&args);
	}
	return (closing_minishell(is_piped, &args));
}
