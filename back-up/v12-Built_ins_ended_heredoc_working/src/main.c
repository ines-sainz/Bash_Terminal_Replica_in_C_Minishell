/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen <danjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 15:25:44 by danjimen          #+#    #+#             */
/*   Updated: 2024/09/04 21:51:56 by danjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

volatile sig_atomic_t	g_signal_received = 0;

void	free_at_exit(t_args *args)
{
	// int	i;

	if (args->input)
	{
		free (args->input);
		args->input = NULL;
	}
	// i = 0;
	// if (args->args[i])
	// {
	// 	while (args->args[i])
	// 		free (args->args[i++]);
	// 	//free (args->args);
	// }
	if (args->arg)
	{
		free (args->arg);
		args->arg = NULL;
	}
	if (args->result)
	{
		free (args->result);
		args->result = NULL;
	}
	if (args->mini->user_prompt)
	{
		free (args->mini->user_prompt);
		args->mini->user_prompt = NULL;
	}
	if (args->last_history)
	{
		free(args->last_history);
		args->last_history = NULL;
	}
	free_env(args->mini);
	/* if (args->arg_ptr)
		free (args->arg_ptr); */
	del_params(args);
	printf("Resources freed successfully.\n");
	clear_history();
	//exit(i);
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
	//printf("\nCaught signal %d (Ctrl-C). Exiting...\n", sig);
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

// Manejador de la señal SIGQUIT (Ctrl-\)
/* void	signal_sigquit(int sig)
{
	printf("\nCaught signal %d (Ctrl-\\). Dumping core and exiting...\n", sig);
	clear_history();
	signal(sig, SIG_DFL); // Restaurar el comportamiento por defecto
	kill(getpid(), sig); // Enviar la señal nuevamente
} */

// Manejador de la señal SIGQUIT (Ctrl-\)
/* void	signal_sigquit(int sig)
{
	//(void)sig;
	printf("XD\n");
	g_signal_received = sig;
	//exit(0);
	//signal(SIGQUIT, SIG_IGN);
} */

// Manejador de la señal SIGQUIT (Ctrl-\)
void	signal_sigquit(int sig)
{
	//printf("\nCaught signal %d (Ctrl-\\). Dumping core and exiting...\n", sig);
	g_signal_received = sig;
	//printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	//clear_history();
	//signal(SIGQUIT, SIG_DFL); // Restaurar el comportamiento por defecto
	//kill(getpid(), SIGQUIT); // Enviar la señal nuevamente
}

// Manejo del EOF
void handle_eof(void)
{
	g_signal_received = -1; // Usamos -1 para indicar EOF
}

// cc signals.c -lreadline
int	main(int argc, char **argv, char **env)
{
	//char	*input;
	t_args	args;
	char	*entry;
	//char	*user_prompt;
	t_mini	mini;

	//Inicializar la estructura y el environment
	ft_bzero(&args, sizeof(t_args));
	ft_bzero(&mini, sizeof(t_mini));
	ft_set_env(env, &mini);
	args.mini = &mini;
	//ft_print_env(&mini);
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
	signal(SIGQUIT, signal_sigquit);

	//Built-ins
	//printf("%s\n", expander("$INES"));
//	printf("pwd: %s\n", buit_ins("pwd", "", &mini)); //funciona
//	printf("env: %s\n", buit_ins("env", "", &mini)); //funciona
//	printf("cd: %s\n", buit_ins("cd", "..", &mini));

	//GET $$ = PID
	// char	*itoa_pid;
	// char	*pid_env;

	// itoa_pid = ft_itoa(getenv());
	// pid_env = ft_strjoin("$=", itoa_pid);
	// ft_export_env(pid_env, &mini);
	// free(pid_env);

	//GET $? = Exit return
	ft_export_env("?=0", &mini);

	//Get SHLVL
	/* ft_export_env("MY_SHLVL=0", &mini);
	if (!ft_strcmp(getenv("SHLVL"), ft_find_env(&mini, "MY_SHLVL")));
		ft_export_env("MY_SHLVL=1", &mini); */

	// Bucle principal del shell
	while (1)
	{
		args.input = readline(mini.user_prompt);
		if (!args.input)
		{
			// Detectar Ctrl-D (EOF)
			handle_eof();
			printf("\nCaught EOF (Ctrl-D). Exiting...\n");
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
		//IMPORTANTE actualizar el estado de las redirecciones
		//Cuento que en este momento esté todo cerrado
		//dup2(0, 0);
		//dup2(1, 1);

		// Procesar la entrada del usuario
		// OJO: ft_strtrim utiliza malloc!!!!!
		args.input_trimed = ft_strtrim(args.input, " \t\n\r\f\v");
		free(args.input);
		args.input = args.input_trimed;
		args.input_trimed = NULL;
		/* if (ft_strcmp(args.input, "exit") == 0)
			exit (0); */
		if (args.input[0] != '\0')
			parse(&args, &mini);
		/* if (args.input[0] != '\0')
		{
			if (parse(&args, &mini) == ERR)
				free_at_exit(&args);
		} */

		free(args.input); // Liberar la memoria asignada por readline
		//free(args.input_trimed); // Liberar la memoria asignada por ft_strtrim
		del_params(&args);
	}
	// Esto solo se ejecuta cuando recibamos Ctrl-D
	//free(entry);
	//free(mini.user_prompt);
	//free (last_history);
	//clear_history();
	printf("\n¡¡EXCLUSIVO PARA CTRL + D!!\n");
	free_at_exit(&args);
	return (0);
}
