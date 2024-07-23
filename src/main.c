/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen & isainz-r <danjimen & isainz-    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 15:25:44 by danjimen          #+#    #+#             */
/*   Updated: 2024/07/23 13:00:26 by danjimen &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
	signal(sig, SIG_DFL);  // Restaurar el comportamiento por defecto
	kill(getpid(), sig);   // Enviar la señal nuevamente
}

// cc signals.c -lreadline
int	main(int argc, char **argv, char **env)
{
	//char	*input;
	t_args	args;
	char	*entrada;
	char	*user_prompt;
	t_mini	mini;

	//environment
	(void)argv;
	if (argc != 1)
		return (1);

	entrada = ft_strjoin(getenv("USER"), "@minishell> ");

	user_prompt = malloc(ft_strlen(RED) + ft_strlen(BOLD) + ft_strlen(entrada) + ft_strlen(RESET) + 1);
	if (!user_prompt)
	{
		fprintf(stderr, "Error allocating memory\n");
		return (1);
	}
	ft_strcpy(user_prompt, RED);
	ft_strlcat(user_prompt, BOLD, ft_strlen(RED) + ft_strlen(BOLD) + ft_strlen(entrada) + ft_strlen(RESET) + 1);
	ft_strlcat(user_prompt, entrada, ft_strlen(RED) + ft_strlen(BOLD) + ft_strlen(entrada) + ft_strlen(RESET) + 1);
	ft_strlcat(user_prompt, RESET, ft_strlen(RED) + ft_strlen(BOLD) + ft_strlen(entrada) + ft_strlen(RESET) + 1);
	//strcat(user_prompt, entrada);
	//strcat(user_prompt, RESET);

	// Configurar los manejadores de señal
	signal(SIGINT, signal_sigint);
	signal(SIGQUIT, signal_sigquit);

	//Inicializar la estructura y el environment
	ft_bzero(&args, sizeof(t_args));
	ft_bzero(&mini, sizeof(t_mini));
	ft_set_env(env, &mini);
	//ft_print_env(&mini);

	// Bucle principal del shell
	while (1)
	{
		args.input = readline(user_prompt);
		if (!args.input)
		{
			// Detectar Ctrl-D (EOF)
			printf("\nCaught EOF (Ctrl-D). Exiting...\n");
			break ;
		}
		if (args.input[0] != '\0')
			add_history(args.input);

		// Procesar la entrada del usuario
		//printf("You entered: %s\n", input);
		
		if (ft_strcmp(ft_strtrim(args.input, " "), "exit") == 0)
			exit (0);
		parse(&args, &mini);

		// Da error desde que está en la estructura t_args
		free(args.input);  // Liberar la memoria asignada por readline
	}
	free(entrada);
	free(user_prompt);
	clear_history();
	return (0);
}
