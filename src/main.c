/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen & isainz-r <danjimen & isainz-    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 15:25:44 by danjimen          #+#    #+#             */
/*   Updated: 2024/10/14 09:19:01 by danjimen &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

volatile sig_atomic_t	g_signal_received = 0;

void	free_at_exit(t_args *args)
{
	close(args->mini->standard_fds[0]);
	close(args->mini->standard_fds[1]);
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
	free_env(args->mini);
	del_params(args);
	printf("DB: Resources freed successfully.\n");
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
		printf(": Not flags are allowed\n");
		exit (127);
	}
}

// Manejador de la señal SIGINT (Ctrl-C)
void	signal_sigint(int sig)
{
	//(void)sig;
	g_signal_received = sig;
	/*printf("\nCaught signal %d (Ctrl-C). Exiting...\n", sig);
	printf("\n");*/
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_signal_received = 0;
}

// Manejador de la señal SIGQUIT (Ctrl-\)
/* void	signal_sigquit(int sig)
{
	//printf("\nCaught signal %d (Ctrl-\\). Dumping core and exiting...\n", sig);
	g_signal_received = sig;
	//printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_signal_received = 0;
	//clear_history();
	//signal(SIGQUIT, SIG_DFL); // Restaurar el comportamiento por defecto
	//kill(getpid(), SIGQUIT); // Enviar la señal nuevamente
} */

// Manejo del EOF
void	handle_eof(void)
{
	g_signal_received = -1; // Usamos -1 para indicar EOF
}

// cc signals.c -lreadline
int	main(int argc, char **argv, char **env)
{
	t_args	args;
	char	*entry;
	t_mini	mini;
	int		i;
	//char	*input;
	//char	*user_prompt;

	//Inicializar la estructura y el environment
	ft_bzero(&args, sizeof(t_args));
	ft_bzero(&mini, sizeof(t_mini));
	ft_set_env(env, &mini);
	args.mini = &mini;
	//environment
	error_mini_use(argc, argv);
	if (getenv("USER") == NULL)
		entry = ft_strdup("user@minishell> ");
	else
		entry = ft_strjoin(getenv("USER"), "@minishell> ");
	mini.user_prompt = malloc(ft_strlen(RED) + ft_strlen(BOLD) + ft_strlen(entry) + ft_strlen(RESET) + 1);
	if (!mini.user_prompt)
	{
		fprintf(stderr, "Error allocating memory\n");
		return (1);
	}
	ft_strcpy(mini.user_prompt, RED);
	ft_strlcat(mini.user_prompt, BOLD, ft_strlen(RED) + ft_strlen(BOLD) + ft_strlen(entry) + ft_strlen(RESET) + 1);
	ft_strlcat(mini.user_prompt, entry, ft_strlen(RED) + ft_strlen(BOLD) + ft_strlen(entry) + ft_strlen(RESET) + 1);
	ft_strlcat(mini.user_prompt, RESET, ft_strlen(RED) + ft_strlen(BOLD) + ft_strlen(entry) + ft_strlen(RESET) + 1);
	free (entry);
	//strcat(user_prompt, entrada);
	//strcat(user_prompt, RESET);

	// Configurar los manejadores de señal
	signal(SIGINT, signal_sigint);
	//signal(SIGQUIT, signal_sigquit);
	signal(SIGQUIT, SIG_IGN);

	//GET $$ = PID
	// char	*itoa_pid;
	// char	*pid_env;

	/* itoa_pid = ft_itoa(getenv());
	   pid_env = ft_strjoin("$=", itoa_pid);
	   ft_export_env(pid_env, &mini);
	   free(pid_env);*/

	//GET $? = Exit return
	ft_export_env("?=0", &mini);

	//Get SHLVL
	/* ft_export_env("MY_SHLVL=0", &mini);
	if (!ft_strcmp(getenv("SHLVL"), ft_find_env(&mini, "MY_SHLVL")));
		ft_export_env("MY_SHLVL=1", &mini); */

	mini.standard_fds[0] = dup(STDIN_FILENO);
	mini.standard_fds[1] = dup(STDOUT_FILENO);

	// Bucle principal del shell
	while (1)
	{
		dup2(mini.standard_fds[0], STDIN_FILENO);
		dup2(mini.standard_fds[1], STDOUT_FILENO);
		g_signal_received = 0;
		args.input = readline(mini.user_prompt);
		if (!args.input)
		{
			// Detectar Ctrl-D (EOF)
			handle_eof();
			//printf("\nDB: Caught EOF (Ctrl-D). Exiting...\n");
			break ;
		}
		/* if (g_signal_received == SIGQUIT)
		{
			printf("\nCaught signal %d (Ctrl-\\). Dumping core and exiting...\n", SIGQUIT);
			printf("\nEsto debería hacerse\n");
			//free_at_exit(&args);
			//signal(SIGQUIT, SIG_DFL); // Restaurar el comportamiento por defecto
			//kill(getpid(), SIGQUIT); // Enviar la señal nuevamente
		} */
		//if (args.input[0] != '\0')
		if ((args.input[0] != '\0' && ft_strcmp(args.input, args.last_history) != 0) || args.last_history == NULL)
		{
			if (args.last_history != NULL)
				free (args.last_history);
			args.last_history = ft_strdup(args.input);
			add_history(args.input);
		}
		// Procesar la entrada del usuario
		// OJO: ft_strtrim utiliza malloc!!!!!
		args.input_trimed = ft_strtrim(args.input, " \t\n\r\f\v");
		free(args.input);
		args.input = args.input_trimed;
		args.input_trimed = NULL;
		/* if (ft_strcmp(args.input, "exit") == 0)
			exit (0); */
		if (args.input != NULL && args.input[0] != '\0')
			parse(&args, &mini);
		signal(SIGINT, signal_sigint);
		/* if (args.input[0] != '\0')
		{
			if (parse(&args, &mini) == ERR)
				free_at_exit(&args);
		} */
		i = 0; // En caso de que haya un error de sintaxis
		if (args.args[i]) // Podría ponerse si parse() devuelve ERR
		{
			while (args.args[i])
			{
				printf("DB: Libero %s por syntax error\n", args.args[i]);
				free(args.args[i]);
				args.args[i] = NULL;
				i++;
			}
		}
		free(args.input); // Liberar la memoria asignada por readline
		//free(args.input_trimed); // Liberar la memoria asignada por ft_strtrim
		del_params(&args);
		// Cerrar descriptores originales
		/* close(mini.standard_fds[0]);
		close(mini.standard_fds[1]); */
	}
	printf("exit\n");
	free_at_exit(&args);
	return (0);
}
