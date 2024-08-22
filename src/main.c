/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen <danjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 15:25:44 by danjimen          #+#    #+#             */
/*   Updated: 2024/08/22 12:53:02 by danjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
	(void)sig;
	//printf("\nCaught signal %d (Ctrl-C). Exiting...\n", sig);
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

// Manejador de la señal SIGQUIT (Ctrl-\)
void	signal_sigquit(int sig)
{
	printf("\nCaught signal %d (Ctrl-\\). Dumping core and exiting...\n", sig);
	clear_history();
	signal(sig, SIG_DFL); // Restaurar el comportamiento por defecto
	kill(getpid(), sig); // Enviar la señal nuevamente
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
		entry = "user@minishell> ";
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
//	printf("pwd: %s\n", buit_ins("pwd", "", &mini)); //funciona
//	printf("env: %s\n", buit_ins("env", "", &mini)); //funciona
	//GET $$ = PID
	char	*pid_env = ft_strjoin("$=", ft_itoa(getpid()));
	ft_export_env(pid_env, &mini);
	free(pid_env);

	// Bucle principal del shell
	while (1)
	{
		args.input = readline(mini.user_prompt);
		if (!args.input)
		{
			// Detectar Ctrl-D (EOF)
			printf("\nCaught EOF (Ctrl-D). Exiting...\n");
			break ;
		}
		if (args.input[0] != '\0')
			add_history(args.input);
		//IMPORTANTE actualizar el estado de las redirecciones
		//Cuento que en este momento esté todo cerrado
		mini.in_out.inf = 0;
		mini.in_out.outf = 1;

		// Procesar la entrada del usuario
		// OJO: ft_strtrim utiliza malloc!!!!!
		args.input_trimed = ft_strtrim(args.input, " ");
		if (ft_strcmp(args.input_trimed, "exit") == 0)
			exit (0);
		if (args.input_trimed[0] != '\0')
			parse(&args, &mini);

		free(args.input); // Liberar la memoria asignada por readline
		free(args.input_trimed); // Liberar la memoria asignada por ft_strtrim

	}
	free(entry);
	free(mini.user_prompt);
	clear_history();
	return (0);
}
